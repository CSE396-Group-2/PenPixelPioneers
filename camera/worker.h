#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include "textsimulation.h"
#include "fontrenderer.h"

class Worker : public QObject
{
    Q_OBJECT

public:
    explicit Worker(QString username, QString textInput, QString serialPort, QLabel* statusLabel, QObject *parent = nullptr);

public slots:
    void CreateText();
    void CreateSerialData();
    void stopWork();

private:
    QString m_username;
    QString m_textInput;
    QString m_serial;
    QLabel* m_statusLabel;

    FontRenderer *fontRenderer;
    TextSimulation *textSimulation;

    bool stopSignal;

signals:
    void finished();
    void updateStatus(const QString& message);

};

#endif // WORKER_H
