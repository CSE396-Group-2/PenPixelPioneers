#include "fontrenderer.h"
#include <QLabel>
#include <QFontDatabase>

FontRenderer::FontRenderer(QObject *parent) : QObject(parent) {}

FontRenderer::FontRenderer(int font_size, std::string font_path, QString text_input, QString writer_name, QObject *parent)
    : QObject(parent), font_size(font_size), font_path(font_path), text_input(text_input), writer_name(writer_name) {}

void FontRenderer::renderFonts() {
    std::cout << this->font_path << std::endl;
    int fontId = QFontDatabase::addApplicationFont(QString::fromStdString(this->font_path));

    if (fontId != -1) {
        qDebug() << "Font loaded successfully.";
    } else {
        qDebug() << "Failed to load font.";
    }

    QLabel label;
    // std::cout << this->text_input.toStdString() << std::endl;
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
    qDebug() << "Text saved as image to: ../camera/data/data_single/text_to_write.jpeg";

    emit finished();
}
