#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <opencv2/opencv.hpp>
#include "../camera/data/single_include/nlohmann/json.hpp"
#include <QMessageBox>

#include <algorithm>

#include "sheettopng.hpp"
using namespace std;
namespace fs = std::filesystem;
// for convenience
using json = nlohmann::json;

// Seq: A-Z, a-z, 0-9, SPECIAL_CHARS
std::vector<int> ALL_CHARS = [] {
    std::vector<int> chars;
    chars.reserve(26 + 26 + 10 + 13);
    for (int i = 65; i <= 90; ++i) {
        chars.push_back(i);
    }
    for (int i = 97; i <= 122; ++i) {
        chars.push_back(i);
    }
    for (int i = 48; i <= 57; ++i) {
        chars.push_back(i);
    }
    const std::string special_chars = ".,;:!?\"'-+=/%&()[]";
    for (char c : special_chars) {
        chars.push_back(static_cast<int>(c));
    }
    return chars;
}();

bool SHEETtoPNG::convert(const std::string& sheet, const std::string& characters_dir, const std::string& config, int cols, int rows) {
    int threshold_value = 200;
    std::ifstream config_file(config);
    if (config_file.is_open()) {
        json j;
        config_file >> j;
        threshold_value = j.value("threshold_value", 200);
        config_file.close();
    } else {
        std::cerr << "Unable to open config file: " << config << std::endl;
        return false;
    }
    if (fs::is_directory(sheet)) {
        throw std::invalid_argument("Sheet parameter should not be a directory.");
    }

    std::vector<std::vector<cv::Mat>> characters = detect_characters(sheet, threshold_value, cols, rows);
    if (characters.size() == 0) {
        QMessageBox msgBox;
        msgBox.setText("Error in template. Please fill it again!");
        msgBox.exec();
        return false;
    }
    save_images(characters, characters_dir);
    return true;
}


std::vector<std::vector<cv::Mat>> SHEETtoPNG::detect_characters(const std::string& sheet_image, int threshold_value, int cols, int rows) {
    std::vector<std::vector<cv::Mat>> sorted_characters;
    cv::Mat image = cv::imread(sheet_image);
    // Check if the image is successfully loaded
    if (image.empty()) {
        return sorted_characters;
    }
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::Mat thresh;
    cv::threshold(gray, thresh, threshold_value, 255, 1);
    cv::Mat close_kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::Mat close;
    cv::morphologyEx(thresh, close, cv::MORPH_CLOSE, close_kernel, cv::Point(-1, -1), 2);
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(close, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    vector<vector<cv::Point>> filteredContours;
    for (const auto& cnt : contours) {
        vector<cv::Point> approx;
        approxPolyDP(cnt, approx, 0.01 * arcLength(cnt, true), true);

        if (approx.size() == 4) {
            filteredContours.push_back(cnt);
        }
    }

    // Filtering contours
    int spaceH = 7 * cv::boundingRect(filteredContours[0]).height / 16;
    int spaceW = 7 * cv::boundingRect(filteredContours[0]).width / 16;

    cout << "filteredContours.size(): " << filteredContours.size() << endl;

    if (filteredContours.size() != rows * cols) {
        return sorted_characters;
    }

    cout << "geldi" << endl;

    vector<vector<cv::Mat>> characters;
    for (int i = 0; i < rows * cols; ++i) {
        cv::Rect boundingRect = cv::boundingRect(filteredContours[i]);
        int cx = boundingRect.x + boundingRect.width / 2;
        int cy = boundingRect.y + boundingRect.height / 2;

        cv::Mat roi = image(cv::Rect(cx - spaceW, cy - spaceH, 2 * spaceW, 2 * spaceH));

        characters.push_back({roi, cv::Mat(1, 1, CV_8U, cv::Scalar(cx)), cv::Mat(1, 1, CV_8U, cv::Scalar(cy))});
    }

    for (size_t k = 0; k < rows; ++k) {
        auto begin = characters.begin() + cols * k;
        auto end = characters.begin() + cols * (k + 1);
        std::sort(begin, end, [](const auto& a, const auto& b) {
            return a[0].cols < b[0].cols;
        });
        sorted_characters.insert(sorted_characters.end(), begin, end);
    }

    return sorted_characters;
}

void SHEETtoPNG::save_images(const std::vector<std::vector<cv::Mat>>& characters, const std::string& characters_dir) {
    string characters_dir2  = characters_dir;
    std::cout << characters_dir2 << std::endl;
    fs::create_directories(characters_dir2);

    for (size_t k = 0; k < characters.size(); ++k) {
        std::string character_dir = (fs::path(characters_dir2) / std::to_string(ALL_CHARS[ALL_CHARS.size() - k - 1])).string();

        fs::create_directories(character_dir);
        // cv::imshow("chars", characters[k][0]);
        // cv::waitKey(0);
        cv::imwrite((fs::path(character_dir) / (std::to_string(ALL_CHARS[ALL_CHARS.size() - k - 1]) + ".png")).string(), characters[k][0]);
    }
}
