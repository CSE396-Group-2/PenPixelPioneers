#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
#include <opencv2/opencv.hpp>

namespace fs = std::filesystem;
using namespace cv;
using namespace std;

void displayImage(const string& name, const Mat& img, bool wait = true);
void copyFile(const fs::path& src, const fs::path& dst);
vector<vector<string>> chunk(const vector<string>& l, int n);
pair<int, int> calculateAccuracy();
void listTestDirectoryContent(const fs::path& directory, const fs::path& output_file);
void printWrongTestcases();
void saveWrongTestcases();

#endif // UTILS_H
