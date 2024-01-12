#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#include <algorithm>
#include <random>
#include <opencv2/opencv.hpp>

#include "utils.h"
#include "constants.h"
#include "pre_processor.cpp"
#include "line_segmentor.cpp"
#include "feature_extractor.cpp"

#include <QProcess>
#include <QDebug>
#include <iostream>
#include <unistd.h>
#include <QMessageBox>

namespace fs = std::filesystem;
using namespace std;
using namespace cv;
class IdentifyWriter {
private:
    ofstream result_file;
    double total_time;
    vector<double> testcase_time;

    cv::Mat img, imggryblr, imgcany;
    cv::Mat imgcontr;
    cv::Mat imgwarp;
    cv::Mat imgclr;
    std::vector<cv::Point> imgpoint;
    std::vector<cv::Point> newpoints;
    float w = 1840, h = 2079;

public:
    IdentifyWriter() : total_time(0.0) {}

    string run(string method) {
        auto start = chrono::steady_clock::now();

        char currentDir[PATH_MAX]; // PATH_MAX is a constant representing the maximum path length
        if (getcwd(currentDir, sizeof(currentDir)) != nullptr) {
            std::cout << "Current directory: " << currentDir << std::endl;
        } else {
            perror("getcwd() error");
            //return 1;
        }

        result_file.open(PREDICTED_RESULTS_PATH);
        string writername = "";
        for (const auto& entry : fs::directory_iterator(TESTCASES_PATH)) {
            if (entry.is_directory()) {
                cout << "Running test iteration '" << entry.path().filename() << "'..." << endl;

                auto t_start = chrono::steady_clock::now();
                if(method == "paragraph") {
                    writername = process_testcase(entry.path().string());
                } else {
                    writername = process_testcase_line_by_line(entry.path().string());
                }

                auto t_end = chrono::steady_clock::now();
                double t = chrono::duration_cast<chrono::seconds>(t_end - t_start).count();
                testcase_time.push_back(t);

                cout << "Finish test iteration '" << entry.path().filename() << "' in " << t << " seconds\n" << endl;
            }
        }

        result_file.close();

        auto end = chrono::steady_clock::now();
        total_time = chrono::duration_cast<chrono::seconds>(end - start).count();
        return writername;
    }

    bool scanImage(const String a, const String b) {
        QProcess process;
        const QString scriptPath = "../camera/bg_remover.py";
        QString argument = QString::fromStdString(a);
        QString argument2 = QString::fromStdString(b);
        QStringList command;
        command << scriptPath << argument << argument2;
        process.start("python3", command);

        // Convert QStringList to QString
        QString joinedString = command.join(" ");

        // Convert QString to std::string
        std::string stdString = joinedString.toStdString();

        cout << stdString << endl;
        QByteArray output;
        // Connect the readyReadStandardOutput signal to a slot
        QObject::connect(&process, &QProcess::readyReadStandardOutput, [&]() {
            // Read the standard output of the process
            output = process.readAllStandardOutput();

            // Print the output to the application output
            qDebug() << "Python Script Output: " << output;
        });

        // Connect the finished signal to a slot
        QObject::connect(&process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [&](int exitCode, QProcess::ExitStatus exitStatus) {
            qDebug() << "Python Script Finished with Exit Code:" << exitCode;
        });

        // Wait for the process to finish (optional)
        process.waitForFinished();

        // Check the exit status
        if (process.exitCode() == 0 && output.toStdString() == "True\n") {
            qDebug() << "Python script executed successfully.";
            return true;
        } else {
            qDebug() << "Error executing Python script. Exit code:" << process.exitCode();
            qDebug() << "Error details:" << process.errorString();
            qDebug() << "Standard Error Output:" << process.readAllStandardError();
            QMessageBox msgBox;
            msgBox.setText("Test image couldn't scanned.");
            msgBox.exec();
            return false;

        }
    }

    string process_testcase(const string& path) {
        // cout << "process_testcase gelen path: " << path << endl;
        vector<vector<double>> features;
        vector<string> labels;

        std::unordered_map<std::string, int> label_mapping;
        int current_label = 0;

        // Loop over every writer in the current test iteration
        for (const auto& entry : fs::directory_iterator(path)) {
            if (entry.is_directory()) {
                fs::path entryPath(entry.path());  // Convert the input string to a filesystem path
                const string writer_id = entryPath.filename().string();

                label_mapping[writer_id] = current_label++;
                cout << "    Processing writer '" << writer_id << "'..." << endl;
                auto [x, y] = get_writer_features(entry.path().string(), writer_id);
                features.insert(features.end(), x.begin(), x.end());
                labels.insert(labels.end(), y.begin(), y.end());
            }
        }

        // Train the KNN model
        cv::Ptr<cv::ml::KNearest> classifier = cv::ml::KNearest::create();
        classifier->setAlgorithmType(cv::ml::KNearest::BRUTE_FORCE);
        classifier->setDefaultK(1);

        // Train the KNN model
        Mat train_data(features.size(), features[0].size(), CV_32F);
        Mat train_labels(labels.size(), 1, CV_32S);
        for (size_t i = 0; i < features.size(); ++i) {
            for (size_t j = 0; j < features[0].size(); ++j) {
                train_data.at<float>(i, j) = features[i][j];
            }
            train_labels.at<int>(i, 0) = label_mapping[labels[i]];
        }

        classifier->train(train_data, cv::ml::ROW_SAMPLE, train_labels);

        vector<string> filenames;

        // Collect filenames
        for (const auto& entry : fs::directory_iterator(path)) {
            if (entry.is_regular_file()) {
                filenames.push_back(entry.path().string());
            }
        }

        // Sort filenames alphabetically
        sort(filenames.begin(), filenames.end());

        string predictedWriter = "";
        for (size_t i = 0; i < filenames.size(); ++i) {
            const auto& filename = filenames[i];
            cout << "filename: " << filename << endl;
            bool is_scanned = scanImage(filename, filename);

            if (!is_scanned)
                break;
            vector<vector<double>> x = get_features(filename);
            size_t num_features = x[0].size();

            cv::Mat test_data(1, num_features, CV_32F);

            for (int j = 0; j < 1; j++) {
                for (size_t k = 0; k < num_features; ++k) {
                    test_data.at<float>(j, k) = static_cast<float>(x[j][k]);
                }
            }

            cv::Mat response;
            classifier->findNearest(test_data, 1, response);
            int predictedLabel = static_cast<int>(response.at<float>(0, 0));

            for (const auto& pair : label_mapping) {
                if (pair.second == predictedLabel) {
                    predictedWriter = pair.first;
                    break;
                }
            }

            cout << "Classifying test image '" << filename << "' as writer " << predictedWriter << "..." << endl;
        }
        return predictedWriter;
    }

    string process_testcase_line_by_line(const string& path){
        vector<vector<double>> features;
        vector<string> labels;

        std::unordered_map<std::string, int> label_mapping;
        int current_label = 0;

        // Loop over every writer in the current test iteration
        for (const auto& entry : fs::directory_iterator(path)) {
            if (entry.is_directory()) {
                fs::path entryPath(entry.path());  // Convert the input string to a filesystem path
                const string writer_id = entryPath.filename().string();

                label_mapping[writer_id] = current_label++;
                cout << "    Processing writer '" << writer_id << "'..." << endl;
                auto [x, y] = get_writer_features(entry.path().string(), writer_id);
                features.insert(features.end(), x.begin(), x.end());
                labels.insert(labels.end(), y.begin(), y.end());
            }
        }

        // Train the KNN model
        cv::Ptr<cv::ml::KNearest> classifier = cv::ml::KNearest::create();
        classifier->setAlgorithmType(cv::ml::KNearest::BRUTE_FORCE);
        classifier->setDefaultK(1);

        // Train the KNN model
        Mat train_data(features.size(), features[0].size(), CV_32F);
        Mat train_labels(labels.size(), 1, CV_32S);
        for (size_t i = 0; i < features.size(); ++i) {
            for (size_t j = 0; j < features[0].size(); ++j) {
                train_data.at<float>(i, j) = features[i][j];
            }
            train_labels.at<int>(i, 0) = label_mapping[labels[i]];
        }

        classifier->train(train_data, cv::ml::ROW_SAMPLE, train_labels);

        vector<string> filenames;

        // Collect filenames
        for (const auto& entry : fs::directory_iterator(path)) {
            if (entry.is_regular_file()) {
                filenames.push_back(entry.path().string());
            }
        }

        // Sort filenames alphabetically
        sort(filenames.begin(), filenames.end());

        // Process sorted filenames
        int correctPredictions = 0;

        string predictedWriter = "";
        string predictedWriters = "";
        for (size_t i = 0; i < filenames.size(); ++i) {
            predictedWriters = "";
            const auto& filename = filenames[i];
            cout << "filename: " << filename << endl;
            scanImage(filename, filename);

            vector<cv::Mat> gray_lines, bin_lines;

            // Load and preprocess the image
            cv::Mat image = cv::imread(filename, cv::IMREAD_GRAYSCALE);

            PreProcessor pre_processor;
            auto result_pair = pre_processor.process(image);
            cv::Mat gray_image = result_pair.first;
            cv::Mat bin_image = result_pair.second;

            LineSegmentor line_segmentor(gray_image, bin_image);
            auto result_pair2 = line_segmentor.segment();
            gray_lines = result_pair2.first;
            bin_lines = result_pair2.second;

            for (size_t i = 0; i < gray_lines.size(); ++i) {
                cv::Mat g = gray_lines[i];
                cv::Mat b = bin_lines[i];

                FeatureExtractor feature_extractor(g, b);
                vector<double> f = feature_extractor.extract();
                size_t num_features = f.size();

                cv::Mat test_data(1, num_features, CV_32F);

                for (int j = 0; j < 1; j++) {
                    for (size_t k = 0; k < num_features; ++k) {
                        test_data.at<float>(j, k) = static_cast<float>(f[k]);
                    }
                }

                cv::Mat response;
                classifier->findNearest(test_data, 1, response);
                int predictedLabel = static_cast<int>(response.at<float>(0, 0));

                for (const auto& pair : label_mapping) {
                    if (pair.second == predictedLabel) {
                        predictedWriter = pair.first;
                        break;
                    }
                }

                cout << "Classifying test image '" << filename << "' as writer " << predictedWriter << "..." << endl;

                if(i % 5 == 0) {
                    predictedWriters += "\n";
                }
                predictedWriters += "Line " + std::to_string(i+1) + ": " + predictedWriter + "\t";
            }
        }

        cout << predictedWriters << endl; 
        return predictedWriters;
    }

    pair<vector<vector<double>>, vector<string>> get_writer_features(const string& path, const string& writer_id) {
        //cout << "get_writer_features gelen path: " << path << endl;
        vector<vector<double>> features;
        vector<string> labels;

        vector<vector<cv::Mat>> total_gray_lines, total_bin_lines;
        // Read and append all lines of the writer
        for (const auto& entry : fs::directory_iterator(path)) {
            if (entry.is_regular_file()) {
                const string filename = entry.path().string();
                cout << filename << endl;

                // Preprocess the image
                cv::Mat image = cv::imread(filename, cv::IMREAD_GRAYSCALE);

                PreProcessor pre_processor;
                auto result_pair = pre_processor.process(image);
                cv::Mat gray_image = result_pair.first;
                cv::Mat bin_image = result_pair.second;
                if (gray_image.empty() || bin_image.empty()) {
                    cerr << "Error: Empty images for file: " << filename << endl;
                    continue;  // Skip the rest of the loop for this file
                }

                LineSegmentor line_segmentor(gray_image, bin_image);
                auto result_pair2 = line_segmentor.segment();
                std::vector<cv::Mat> gray_lines = result_pair2.first;
                std::vector<cv::Mat> bin_lines = result_pair2.second;

                total_gray_lines.push_back(gray_lines);
                total_bin_lines.push_back(bin_lines);
            }
        }
        cout << "total_gray_lines.size()" << total_gray_lines[0].size() << endl;
        cout << "total_bin_lines.size()" << total_bin_lines[0].size() << endl;

        for (size_t i = 0; i < total_gray_lines.size(); ++i) {
            for(size_t j = 0; j < total_gray_lines[i].size(); ++j) {
                cv::Mat g = total_gray_lines[i][j];
                cv::Mat b = total_bin_lines[i][j];
                FeatureExtractor feature_extractor(g, b);
                std::vector<double> f = feature_extractor.extract();

                /*cv::imshow("g", g);
                cv::waitKey(0);*/
                features.push_back(f);
                labels.push_back(writer_id);
            }
        }

        return {features, labels};
    }

    vector<vector<double>> get_features(const string& path) {
        vector<cv::Mat> gray_lines, bin_lines;

        // Load and preprocess the image
        cv::Mat image = cv::imread(path, cv::IMREAD_GRAYSCALE);

        PreProcessor pre_processor;
        auto result_pair = pre_processor.process(image);
        cv::Mat gray_image = result_pair.first;
        cv::Mat bin_image = result_pair.second;

        LineSegmentor line_segmentor(gray_image, bin_image);
        auto result_pair2 = line_segmentor.segment();
        gray_lines = result_pair2.first;
        bin_lines = result_pair2.second;

        vector<vector<double>> x;

        for (size_t i = 0; i < gray_lines.size(); ++i) {
            cv::Mat g = gray_lines[i];
            cv::Mat b = bin_lines[i];
            /*cv::imshow("g", g);
            cv::waitKey(0);*/
            FeatureExtractor feature_extractor(g, b);
            vector<double> f = feature_extractor.extract();
            x.push_back(f);
        }

        return x;
    }

};
