#include "worker.h"

Worker::Worker(QString username, QString textInput, QString serialPort, QLabel* statusLabel, QObject *parent)
    : QObject(parent), m_username(username), m_textInput(textInput), m_serial(serialPort), m_statusLabel(statusLabel) {

}

void Worker::CreateText() {
    emit updateStatus("Creating font");

    // Set up FontRenderer object
    int font_size = 50;
    std::string font_path = "../camera/data/ttf/" + m_username.toStdString();
    std::cout << "font path " << font_path << std::endl;

    fontRenderer = new FontRenderer(font_size, font_path, m_textInput, m_username);
    connect(fontRenderer, &FontRenderer::finished, this, &Worker::CreateSerialData);

    fontRenderer->renderFonts();
}

void Worker::CreateSerialData() {
    emit updateStatus("Creating pencil movements");

    // Set up TextSimulation object
    textSimulation = new TextSimulation();
    connect(textSimulation, &TextSimulation::updateStatus, this, &Worker::updateStatus);

    textSimulation->processImage();

    stopSignal = false;
    textSimulation->sendSerialData(m_serial, stopSignal);

    emit finished();
}

void Worker::stopWork() {
    stopSignal = true;
}


