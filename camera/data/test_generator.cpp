#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#include <algorithm>
#include <random>

namespace fs = std::filesystem;
using namespace std;

class TestGenerator {
private:
    string datasetPath = DATASET_PATH + "forms/";
    string datasetMetaFile = DATASET_PATH + "meta.txt";
    unordered_map<string, vector<string>> writers;
    vector<string> writerIds;
    ofstream outputFile;

public:
    TestGenerator() {
        ifstream meta(datasetMetaFile);
        string line;

        while (getline(meta, line)) {
            if (line[0] == '#') continue;

            istringstream iss(line);
            string imageFilename, writerId;
            iss >> imageFilename >> writerId;

            if (writers.find(writerId) == writers.end()) {
                writers[writerId] = vector<string>();
            }
            writers[writerId].push_back(imageFilename);
        }

        for (auto it = writers.begin(); it != writers.end(); ) {
            if (it->second.size() < 3) {
                it = writers.erase(it);
            } else {
                ++it;
            }
        }

        for (const auto& writer : writers) {
            writerIds.push_back(writer.first);
        }
    }

    void generate(const string& path, int n, int m, int k) {
        if (fs::exists(path)) {
            fs::remove_all(path);
        }
        fs::create_directories(path);

        outputFile.open(path + EXPECTED_RESULTS_FILENAME);

        for (int i = 0; i < n; ++i) {
            cout << "Generating test iteration " << i << "..." << endl;
            string testPath = path + to_string(i) + "/";
            generateTest(testPath, m, k);
        }

        outputFile.close();
    }

private:
    void generateTest(const string& path, int m, int k) {
        unordered_map<string, int> usedWriters;
        unordered_set<string> usedImages;
        random_device rd;
        mt19937 gen(rd());

        fs::create_directory(path);

        for (int i = 0; i < m; ++i) {
            string w = getRandomElement(writerIds, gen);
            while (usedWriters.find(w) != usedWriters.end()) {
                w = getRandomElement(writerIds, gen);
            }
            usedWriters[w] = i;

            vector<string>& images = writers[w];

            for (int j = 0; j < k; ++j) {
                string img = getRandomElement(images, gen);
                while (usedImages.find(img) != usedImages.end()) {
                    img = getRandomElement(images, gen);
                }
                usedImages.insert(img);

                string srcImg = datasetPath + img + ".png";
                string dstImg = path + to_string(i) + "/" + img + ".png";
                copyFile(srcImg, dstImg);
            }
        }

        string w = getRandomElement(writerIds, gen);
        vector<string>& images = writers[w];

        string img = getRandomElement(images, gen);
        while (usedImages.find(img) != usedImages.end()) {
            img = getRandomElement(images, gen);
        }

        string srcImg = datasetPath + img + ".png";
        string dstImg = path + img + ".png";
        copyFile(srcImg, dstImg);

        outputFile << usedWriters[w] << endl;
    }

    template<typename T>
    T getRandomElement(const vector<T>& vec, mt19937& gen) {
        uniform_int_distribution<> dis(0, vec.size() - 1);
        return vec[dis(gen)];
    }
};