// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef CAMERA_H
#define CAMERA_H

#include <QAudioInput>
#include <QCamera>
#include <QImageCapture>
#include <QMediaCaptureSession>
#include <QMediaDevices>
#include <QMediaMetaData>
#include <QMediaRecorder>
#include <QScopedPointer>
#include <QFileDialog>
#include <QListWidget>
#include <QPushButton>
#include <QThread>

#include <QMainWindow>
#include <opencv2/opencv.hpp>

#include "worker.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class Camera;
}
class QActionGroup;
QT_END_NAMESPACE

class MetaDataDialog;

class Camera : public QMainWindow
{
    Q_OBJECT

public:
    Camera();

public slots:
    void saveMetaData();
    void updateSerialPortList();

private slots:
    void init();

    void setCamera(const QCameraDevice &cameraDevice);

    void startCamera();
    void stopCamera();

    void record();
    void pause();
    void stop();
    void setMuted(bool);

    void takeImage();
    void displayCaptureError(int, QImageCapture::Error, const QString &errorString);

    void configureCaptureSettings();
    void configureVideoSettings();
    void configureImageSettings();

    void displayRecorderError();
    void displayCameraError();

    void updateCameraDevice(QAction *action);

    void updateCameraActive(bool active);
    void updateCaptureMode();
    void updateRecorderState(QMediaRecorder::RecorderState state);
    void setExposureCompensation(int index);

    void updateRecordTime();

    void processCapturedImage(int requestId, const QImage &img);

    void displayViewfinder();
    void displayCapturedImage();

    void readyForCapture(bool ready);
    void imageSaved(int id, const QString &fileName);

    void updateCameras();

    void showMetaDataDialog();

    void on_changeToDrawButton_clicked();

    void on_changeToAnalyzeButton_clicked();

    void on_ClearButton_clicked();

    void on_SendButton_clicked();

    void on_takeImageButton_clicked();
    void on_takeImageButton_2_clicked();

    QStringList getUserProfiles();
    void generateUserProfileList();

    QStringList getUserFonts();
    void generateUserFontList();

    void on_actionAdd_Profile_triggered();

    void on_actionAdd_Font_triggered();

    void on_goBack_1_clicked();

    void on_goBack_2_clicked();

    void on_saveNewFontButton_clicked();

    void on_saveNewFontImageButton_clicked();

    void on_saveNewProfileImageButton_clicked();

    void fillListView();

    QStringList listSerialPorts();


    void on_saveNewProfileButton_clicked();

    void on_refreshPorts_clicked();

    void on_StopButton_clicked();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::Camera *ui;

    QActionGroup *videoDevicesGroup = nullptr;

    QMediaDevices m_devices;
    QScopedPointer<QImageCapture> m_imageCapture;
    QMediaCaptureSession m_captureSession;
    QScopedPointer<QCamera> m_camera;
    QScopedPointer<QAudioInput> m_audioInput;
    QScopedPointer<QMediaRecorder> m_mediaRecorder;
    QString uploadedFilePath;
    QStringList uploadedFilePaths;

    bool m_isCapturingImage = false;
    bool m_applicationExiting = false;
    bool m_doImageCapture = true;

    MetaDataDialog *m_metaDataDialog = nullptr;

    Worker *worker;
    QThread *thread;

    void stop_ongoing_work();
};

#endif
