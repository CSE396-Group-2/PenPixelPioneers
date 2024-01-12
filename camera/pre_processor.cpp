#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>

#include "utils.h"
#include "constants.h"

class PreProcessor {
public:
    static std::pair<cv::Mat, cv::Mat> process(const cv::Mat& gray_img) {
        // cv::imshow("gray_img process", gray_img);
        // cv::waitKey(0);
        // Reduce image noise.
        cv::Mat blurred_img;
        cv::GaussianBlur(gray_img, blurred_img, cv::Size(5, 5), 0);

        // Initial cropping.
        int l_padding = 150;
        int r_padding = 50;
        cv::Mat cropped_img = blurred_img(cv::Rect(l_padding, 0, blurred_img.cols - l_padding - r_padding, blurred_img.rows));

        // Binarize the image.
        cv::Mat bin_img;
        cv::threshold(cropped_img, bin_img, 0, 255, cv::THRESH_BINARY_INV + cv::THRESH_OTSU);

        // Crop page header and footer and keep only the handwritten area.
        //auto pairs = _crop_paragraph(cropped_img, bin_img);
        // cv::imshow("cropped_img process", cropped_img);
        // cv::waitKey(0);
        //cropped_img = pairs.first;
        //bin_img = pairs.second;

        // Return pre-processed images.
        return std::make_pair(cropped_img, bin_img);
    }

private:
    static std::pair<cv::Mat, cv::Mat> _crop_paragraph(cv::Mat& gray_img, cv::Mat& bin_img) {
        // Find all contours in the page.
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(bin_img, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

        // Minimum contour width to be considered as the black separator line.
        int threshold_width = 1000;
        int line_offset = 10;

        // Page paragraph boundaries.
        int up = 0, down = gray_img.rows - 1, left = 0, right = gray_img.cols - 1;

        // Detect the main horizontal black separator lines of the IAM handwriting forms.
        for (const auto& cnt : contours) {
            cv::Rect boundingRect = cv::boundingRect(cnt);
            int x = boundingRect.x;
            int y = boundingRect.y;
            int w = boundingRect.width;
            int h = boundingRect.height;

            if (w < threshold_width) {
                continue;
            }

            if (y < gray_img.rows / 2) {
                up = std::max(up, y + h + line_offset);
            } else {
                down = std::min(down, y - line_offset);
            }
        }

        // Apply erosion to remove noise and dots.
        cv::Mat kernel = cv::Mat::ones(3, 3, CV_8U);
        cv::Mat eroded_img;
        cv::erode(bin_img, eroded_img, kernel, cv::Point(-1, -1), 2);
        // cv::imshow("bin_img_crop_paragraph", eroded_img);
        // cv::waitKey(0);

        // Get horizontal and vertical histograms
        std::vector<int> hor_hist(eroded_img.rows, 0);
        std::vector<int> ver_hist(eroded_img.cols, 0);


        for (int i = 0; i < eroded_img.rows; ++i) {
            hor_hist[i] = static_cast<int>(cv::sum(eroded_img.row(i))[0] / 255.0);
        }

        for (int j = 0; j < eroded_img.cols; ++j) {
            ver_hist[j] = static_cast<int>(cv::sum(eroded_img.col(j))[0] / 255.0);
        }

        // Detect paragraph white padding.
        while (left < right && ver_hist[left] == 0) {
            left++;
        }
        while (right > left && ver_hist[right] == 0) {
            right--;
        }
        while (up < down && hor_hist[up] == 0) {
            up++;
        }
        while (down > up && hor_hist[down] == 0) {
            down--;
        }

        // Crop images.
        gray_img = gray_img(cv::Range(up, down + 1), cv::Range(left, right + 1));
        bin_img = bin_img(cv::Range(up, down + 1), cv::Range(left, right + 1));

        // cv::imshow("gray_img _crop_paragraph", gray_img);
        // cv::waitKey(0);
        // cv::imshow("bin_img _crop_paragraph", bin_img);
        // cv::waitKey(0);

        //Display bounding box on the handwritten paragraph.
        if (DEBUG_PARAGRAPH_SEGMENTATION) {
            cv::Mat img_color;
            cv::cvtColor(gray_img, img_color, cv::COLOR_GRAY2BGR);
            cv::rectangle(img_color, cv::Point(left, up), cv::Point(right, down), cv::Scalar(0, 0, 255), 3);
            displayImage("Handwritten Paragraph", img_color);
        }
        return std::make_pair(gray_img, bin_img);
    }
};
