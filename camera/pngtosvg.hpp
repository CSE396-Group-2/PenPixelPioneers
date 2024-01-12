#ifndef PNGTOSVG_HPP_
#define PNGTOSVG_HPP_

#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <opencv2/opencv.hpp>
#include <cstdlib>

namespace fs = std::filesystem;

class PNGtoSVG {
public:
    void convert(const std::string& directory);

private:
    void bmpToSvg(const std::string& path);
    void pngToBmp(const std::string& path);
    std::string shutil_which(const std::string& cmd);

    class PotraceNotFound : public std::exception {
    public:
        PotraceNotFound(const char* message) : errorMessage(message) {}

        const char* what() const noexcept override {
            return errorMessage.c_str();
        }

    private:
        std::string errorMessage;
    };
};

#endif // PNGTOSVG_HPP_
