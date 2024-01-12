#ifndef FONTRENDERER_H
#define FONTRENDERER_H

#include <QApplication>
#include <QFont>
#include <QFontDatabase>
#include <QLabel>
#include <QPixmap>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class FontRenderer : public QObject
{
    Q_OBJECT

public:
    explicit FontRenderer(QObject *parent = nullptr);
    FontRenderer(int font_size, std::string font_path, QString text_input, QString writer_name, QObject *parent = nullptr);

    void renderFonts();

signals:
    void finished();

private:
    int font_size;
    std::string font_path;
    QString text_input;
    QString writer_name;
};

#endif // FONTRENDERER_H
