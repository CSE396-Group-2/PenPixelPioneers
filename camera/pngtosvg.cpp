#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <opencv2/opencv.hpp>
#include <cstdlib>

#include "pngtosvg.hpp"

namespace fs = std::filesystem;
using namespace std;


void PNGtoSVG::convert(const std::string& directory) {
    std::string characters_dir2  = directory;
    fs::path dir_path(characters_dir2);
    for (const auto& entry : fs::directory_iterator(dir_path)) {
        string path = entry.path().string();
        //std::cout << "image path: " << path << std::endl;
        // Find the position of the last '/'
        size_t lastSlashPos = path.find_last_of('/');
        int number;

        if (lastSlashPos != std::string::npos) {
            std::string numberString = path.substr(lastSlashPos + 1);
            std::istringstream(numberString) >> number;
        } else {
            std::cout << "No '/' found in the path" << std::endl;
        }
        string new_path = path + "/" + std::to_string(number) + ".png";
        fs::path bmpPath = new_path;
        bmpPath.replace_extension(".bmp");
        pngToBmp(new_path);
        bmpToSvg(bmpPath.string());
    }
}



void PNGtoSVG::bmpToSvg(const std::string& path) {
    if (shutil_which("potrace").empty()) {
        throw PotraceNotFound("Potrace is either not installed or not in path");
    } else {
        std::string svg_path = path.substr(0, path.length() - 4) + ".svg";
        std::string potrace_cmd = "potrace " + path + " -b svg -o " + svg_path;
        std::system(potrace_cmd.c_str());
    }
}

void PNGtoSVG::pngToBmp(const std::string& path) {
  try {
    std::cout << path << std::endl;
    // Read the image using OpenCV
    cv::Mat image = imread(path, cv::IMREAD_UNCHANGED);
    // Resize the image
    cv::resize(image, image, cv::Size(100, 100), cv::INTER_AREA);

    // Convert the image to RGBA format
    cv::Mat bgrImage;
    cv::cvtColor(image, bgrImage, cv::COLOR_BGR2RGBA);

    // Threshold the image to convert each pixel to black or white
    const int threshold = 200;
    std::vector<cv::Vec4b> data(image.rows * image.cols);

    for (int y = 0; y < image.rows; ++y) {
      for (int x = 0; x < image.cols; ++x) {
        cv::Vec4b pixel = bgrImage.at<cv::Vec4b>(y, x);
        if (pixel[0] >= threshold && pixel[1] >= threshold && pixel[2] >= threshold && pixel[3] >= threshold) {
          data[y * image.cols + x] = cv::Vec4b(255, 255, 255, 0);
        } else {
          data[y * image.cols + x] = cv::Vec4b(0, 0, 0, 255); // Change alpha value to 255 for full opacity
        }
      }
    }

    // Set the image data with processed pixels
    cv::Mat newImage(image.rows, image.cols, CV_8UC4, data.data());

    // Save the modified image as a new .bmp file
    std::string newPath = path.substr(0, path.length() - 4) + ".bmp";
    cv::imwrite(newPath, newImage);

  } catch (const std::exception& e) {
    std::cerr << "Error converting image: " << e.what() << std::endl;
  }
}

std::string PNGtoSVG::shutil_which(const std::string& cmd) {

#ifdef _WIN32
    std::string where_cmd = "where " + cmd;
    FILE* pipe = _popen(where_cmd.c_str(), "r");
    if (!pipe) return "";

    char buffer[128];
    std::string result = "";

    while (fgets(buffer, 128, pipe) != nullptr) {
        result += buffer;
    }

    _pclose(pipe);
    return result;
#endif

    FILE* pipe = popen(("which " + cmd).c_str(), "r");
    if (!pipe) return "";

    char buffer[128];
    std::string result = "";

    while (fgets(buffer, 128, pipe) != nullptr) {
        result += buffer;
    }

    pclose(pipe);
    return result;
}
