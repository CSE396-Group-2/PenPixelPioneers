#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <filesystem>

#include "sheettopng.hpp"
#include "pngtosvg.hpp"
#include "svgtottf.hpp"

#include <QMessageBox>

using namespace std;
 
class Converter {
public:
    static int run(const std::string& sheet, const std::string& output_directory, const std::string& characters_dir, std::string& config, const std::string& metadata, const std::string& stylename) {
        int success = 1;

        SHEETtoPNG converter1;
        bool is_sheettopng_converted = converter1.convert(sheet, characters_dir, config, 8, 10);
        if(!is_sheettopng_converted)
            return 1;


        PNGtoSVG converter2;
        converter2.convert(characters_dir);

        SVGtoTTF converter3;
        success = converter3.convert(characters_dir, output_directory, config, metadata, stylename);

        if(success) {
            QMessageBox msgBox;
            msgBox.setText("Error while creating font!");
            msgBox.exec();
        }

        return success;
    }

    static int converters(const std::string& sheet, const std::string& output_directory, const std::string& directory, std::string& config, const std::string& metadata, const std::string& stylename) {
        std::string temp_directory;
        bool is_temp_dir = false;
        int success = 1;
 
        if (directory.empty()) {
            temp_directory = std::filesystem::temp_directory_path().string();
            is_temp_dir = true;
        } else {
            temp_directory = directory;
        }
 
        if (config.empty()) {
            // Assuming the default.json is in the same directory as the executable
            config = "default.json";
        }
 
        if (std::filesystem::is_directory(config)) {
            throw std::invalid_argument("Config parameter should not be a directory.");
        }
 
        if (std::filesystem::is_directory(sheet)) {
            throw std::invalid_argument("Sheet parameter should not be a directory.");
        } else {
            success = run(sheet, output_directory, temp_directory, config, metadata, stylename);
        }

        return success;
    }
};
 /*
int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input_path> <output_directory> [--directory <directory>] [--config <config>] [--filename <filename>] [--family <family>] [--style <style>]\n";
        return EXIT_FAILURE;
    }
 
    std::string sheet = argv[3];
    std::string output_directory = argv[4];
    std::string directory;
    std::string config;
    std::string filename;
    std::string family;
    std::string style;
 
    for (int i = 3; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--directory" && i + 1 < argc) {
            directory = argv[i + 1];
        } else if (arg == "--config" && i + 1 < argc) {
            config = argv[i + 1];
        } else if (arg == "--filename" && i + 1 < argc) {
            filename = argv[i + 1];
        } else if (arg == "--family" && i + 1 < argc) {
            family = argv[i + 1];
        } else if (arg == "--style" && i + 1 < argc) {
            style = argv[i + 1];
        }
    }
 
    std::string metadata = "{\"filename\": \"" + filename + "\", \"family\": \"" + family + "\", \"style\": \"" + style + "\"}";
    Converter::converters(sheet, output_directory, directory, config, metadata);
 
    return EXIT_SUCCESS;
}*/
