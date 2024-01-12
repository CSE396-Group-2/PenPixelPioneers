#ifndef SHEETTOPNG_HPP_
#define SHEETTOPNG_HPP_

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

class SHEETtoPNG {
public:
    bool convert(const std::string& sheet, const std::string& characters_dir, const std::string& config, int cols, int rows);


private:
    std::vector<std::vector<cv::Mat>> detect_characters(const std::string& sheet_image, int threshold_value, int cols, int rows);
    void save_images(const std::vector<std::vector<cv::Mat>>& characters, const std::string& characters_dir);
};

#endif
