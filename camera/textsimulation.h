#pragma once

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

#ifdef _WIN32
#include <Windows.h>
#else
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#endif

#include <fstream>
#include <unordered_set>
#include <QSerialPortInfo>
#include <QMessageBox>

#pragma comment(lib, "SetupAPI.lib")

struct PointHash {
    std::size_t operator()(const cv::Point& point) const {
        return std::hash<int>()(point.x) ^ (std::hash<int>()(point.y) << 1);
    }
};

class TextSimulation : public QObject {
    Q_OBJECT

private:
    cv::Mat image;
    std::vector<cv::Point> pixelCoordinates;

#ifndef _WIN32
    int serial; // File descriptor for Linux serial port
#endif

public:
    TextSimulation();
    void processImage();
    std::vector<cv::Point> findShortestPath(const std::vector<cv::Point>& avaliablePoints,
                                            const cv::Point start,
                                            const cv::Point destination);
    std::vector<cv::Point> reorderIsland(const std::vector<cv::Point>& island);
    void sendSerialData(QString serialPort, bool& stop);

signals:
    void updateStatus(const QString& statusMessage);
};
