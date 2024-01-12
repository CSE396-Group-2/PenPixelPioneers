#include <QApplication>
#include <QFont>
#include <QFontDatabase>
#include <QLabel>
#include <QPixmap>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
class FontRenderer {
public:
    FontRenderer(){}
    FontRenderer(int font_size, std::string font_path, QString text_input, QString writer_name) {
        this->font_path = font_path;
        this->font_size = font_size;
        this->text_input = text_input;
        this->writer_name = writer_name;
    }


    void renderFonts() {
        std::cout << this->font_path << std::endl;
        int fontId = QFontDatabase::addApplicationFont(QString::fromStdString(this->font_path));

        if (fontId != -1) {
            qDebug() << "Font loaded successfully.";
        } else {
            qDebug() << "Failed to load font.";
        }

        QLabel label;
        std::cout << this->text_input.toStdString() << std::endl;
        label.setText(this->text_input);// Set a fixed size for the QLabel
        label.setFixedSize(2000, 500); // Adjust the size as needed
        // Enable word wrapping
        label.setWordWrap(true);
        // Set text color to white and background color to black
        label.setStyleSheet("QLabel { color : white; background-color : black; }");
        // Find the position of the dot in the extension
        std::string filename = this->writer_name.toStdString();
        size_t dotPosition = filename.find_last_of('.');

        // Check if the dot was found
        if (dotPosition != std::string::npos) {
            // Remove the extension by taking a substring up to the dot position
            filename = filename.substr(0, dotPosition);

            // Output the result
            std::cout << "Filename without extension: " << filename << std::endl;
        } else {
            // No dot found, handle the case accordingly
            std::cerr << "Error: No file extension found in the filename." << std::endl;
        }
        std::cout << filename << std::endl;
        label.setFont(QFont(QString::fromStdString(filename), this->font_size)); // "YourCustomFont" is the family name of your font

        // Create an image with the label
        QPixmap pixmap(label.size());
        label.render(&pixmap);

        // Save the image to a file (optional)
        pixmap.save("../camera/data/data_single/text_to_write.jpeg");
    }

private:
    int font_size;
    std::string font_path;
    QString text_input;
    QString writer_name;


    cv::Mat img, imggryblr, imgcany;
    cv::Mat imgcontr;
    cv::Mat imgwarp;
    cv::Mat imgclr;
    std::vector<cv::Point> imgpoint;
    std::vector<cv::Point> newpoints;
    float w = 1840, h = 2079;
};
