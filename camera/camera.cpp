// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "camera.h"
#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
#    include "ui_camera_mobile.h"
#else
#    include "ui_camera.h"
#endif

#include "imagesettings.h"
#include "metadatadialog.h"
#include "videosettings.h"
#include "textsimulation.h"
#include "constants.h"

#include <QAudioDevice>
#include <QAudioInput>
#include <QCameraDevice>
#include <QMediaDevices>
#include <QMediaFormat>
#include <QMediaMetaData>
#include <QMediaRecorder>
#include <QVideoWidget>

#include <QLineEdit>
#include <QMessageBox>

#include <QAction>
#include <QActionGroup>
#include <QImage>
#include <QKeyEvent>
#include <QPalette>

#include <QDir>
#include <QTimer>

#include "identify_writer.cpp"
#include "cli.cpp"

#if QT_CONFIG(permissions)
#include <QPermission>
#endif

Camera::Camera() : ui(new Ui::Camera)
{
    ui->setupUi(this);
    // disable all buttons by default
    updateCameraActive(false);
    readyForCapture(false);
    //ui->recordButton->setEnabled(false);
    //ui->pauseButton->setEnabled(false);
    //ui->stopButton->setEnabled(false);
    //ui->metaDataButton->setEnabled(false);

    generateUserProfileList();
    generateUserFontList();
    updateSerialPortList();

    // Create a QThread
    thread = new QThread();

    // try to actually initialize camera
    init();
}

void Camera::init()
{
#if QT_CONFIG(permissions)
    // camera
    QCameraPermission cameraPermission;
    switch (qApp->checkPermission(cameraPermission)) {
    case Qt::PermissionStatus::Undetermined:
        qApp->requestPermission(cameraPermission, this, &Camera::init);
        return;
    case Qt::PermissionStatus::Denied:
        qWarning("Camera permission is not granted!");
        return;
    case Qt::PermissionStatus::Granted:
        break;
    }
    // microphone
    QMicrophonePermission microphonePermission;
    switch (qApp->checkPermission(microphonePermission)) {
    case Qt::PermissionStatus::Undetermined:
        qApp->requestPermission(microphonePermission, this, &Camera::init);
        return;
    case Qt::PermissionStatus::Denied:
        qWarning("Microphone permission is not granted!");
        return;
    case Qt::PermissionStatus::Granted:
        break;
    }
#endif

    m_audioInput.reset(new QAudioInput);
    m_captureSession.setAudioInput(m_audioInput.get());

    // Camera devices:

    videoDevicesGroup = new QActionGroup(this);
    videoDevicesGroup->setExclusive(true);
    updateCameras();
    connect(&m_devices, &QMediaDevices::videoInputsChanged, this, &Camera::updateCameras);

    connect(videoDevicesGroup, &QActionGroup::triggered, this, &Camera::updateCameraDevice);
    //connect(ui->captureWidget, &QTabWidget::currentChanged, this, &Camera::updateCaptureMode);

    //connect(ui->metaDataButton, &QPushButton::clicked, this, &Camera::showMetaDataDialog);
    //connect(ui->exposureCompensation, &QAbstractSlider::valueChanged, this,
    //&Camera::setExposureCompensation);

    setCamera(QMediaDevices::defaultVideoInput());
}

void Camera::setCamera(const QCameraDevice &cameraDevice)
{
    m_camera.reset(new QCamera(cameraDevice));
    m_captureSession.setCamera(m_camera.data());

    connect(m_camera.data(), &QCamera::activeChanged, this, &Camera::updateCameraActive);
    connect(m_camera.data(), &QCamera::errorOccurred, this, &Camera::displayCameraError);

    if (!m_mediaRecorder) {
        m_mediaRecorder.reset(new QMediaRecorder);
        m_captureSession.setRecorder(m_mediaRecorder.data());
        connect(m_mediaRecorder.data(), &QMediaRecorder::recorderStateChanged, this,
                &Camera::updateRecorderState);
        connect(m_mediaRecorder.data(), &QMediaRecorder::durationChanged, this,
                &Camera::updateRecordTime);
        connect(m_mediaRecorder.data(), &QMediaRecorder::errorChanged, this,
                &Camera::displayRecorderError);
    }

    if (!m_imageCapture) {
        m_imageCapture.reset(new QImageCapture);
        m_captureSession.setImageCapture(m_imageCapture.get());
        connect(m_imageCapture.get(), &QImageCapture::readyForCaptureChanged, this,
                &Camera::readyForCapture);
        connect(m_imageCapture.get(), &QImageCapture::imageCaptured, this,
                &Camera::processCapturedImage);
        connect(m_imageCapture.get(), &QImageCapture::imageSaved, this, &Camera::imageSaved);
        connect(m_imageCapture.get(), &QImageCapture::errorOccurred, this,
                &Camera::displayCaptureError);
    }

    m_captureSession.setVideoOutput(ui->viewfinder);

    updateCameraActive(m_camera->isActive());
    updateRecorderState(m_mediaRecorder->recorderState());
    readyForCapture(m_imageCapture->isReadyForCapture());

    updateCaptureMode();
    m_camera->start();
}

void Camera::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
        return;

    switch (event->key()) {
    case Qt::Key_CameraFocus:
        displayViewfinder();
        event->accept();
        break;
    case Qt::Key_Camera:
        if (m_doImageCapture) {
            takeImage();
        } else {
            if (m_mediaRecorder->recorderState() == QMediaRecorder::RecordingState)
                stop();
            else
                record();
        }
        event->accept();
        break;
    default:
        QMainWindow::keyPressEvent(event);
    }
}

void Camera::updateRecordTime()
{
    QString str = tr("Recorded %1 sec").arg(m_mediaRecorder->duration() / 1000);
    //ui->statusbar->showMessage(str);
}

void Camera::processCapturedImage(int requestId, const QImage &img)
{
    Q_UNUSED(requestId);
    QImage scaledImage =
        img.scaled(ui->viewfinder->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    ui->lastImagePreviewLabel->setPixmap(QPixmap::fromImage(scaledImage));

    // Display captured image for 4 seconds.
    displayCapturedImage();
    QTimer::singleShot(4000, this, &Camera::displayViewfinder);
}

void Camera::configureCaptureSettings()
{
    if (m_doImageCapture)
        configureImageSettings();
    else
        configureVideoSettings();
}

void Camera::configureVideoSettings()
{
    VideoSettings settingsDialog(m_mediaRecorder.data());

    if (settingsDialog.exec())
        settingsDialog.applySettings();
}

void Camera::configureImageSettings()
{
    ImageSettings settingsDialog(m_imageCapture.get());

    if (settingsDialog.exec() == QDialog::Accepted)
        settingsDialog.applyImageSettings();
}

void Camera::record()
{
    m_mediaRecorder->record();
    updateRecordTime();
}

void Camera::pause()
{
    m_mediaRecorder->pause();
}

void Camera::stop()
{
    m_mediaRecorder->stop();
}

void Camera::setMuted(bool muted)
{
    m_captureSession.audioInput()->setMuted(muted);
}

void Camera::takeImage()
{
    m_isCapturingImage = true;
    m_imageCapture->captureToFile();
}

void Camera::displayCaptureError(int id, const QImageCapture::Error error,
                                 const QString &errorString)
{
    Q_UNUSED(id);
    Q_UNUSED(error);
    QMessageBox::warning(this, tr("Image Capture Error"), errorString);
    m_isCapturingImage = false;
}

void Camera::startCamera()
{
    m_camera->start();
}

void Camera::stopCamera()
{
    m_camera->stop();
}

void Camera::updateCaptureMode()
{
    //int tabIndex = ui->captureWidget->currentIndex();
    //m_doImageCapture = (tabIndex == 0);
}

void Camera::updateCameraActive(bool active)
{
    if (active) {
        ui->actionStartCamera->setEnabled(false);
        ui->actionStopCamera->setEnabled(true);
        //ui->captureWidget->setEnabled(true);
        //ui->actionSettings->setEnabled(true);
    } else {
        ui->actionStartCamera->setEnabled(true);
        ui->actionStopCamera->setEnabled(false);
        //ui->captureWidget->setEnabled(false);
        //ui->actionSettings->setEnabled(false);
    }
}

void Camera::updateRecorderState(QMediaRecorder::RecorderState state)
{
    switch (state) {
    case QMediaRecorder::StoppedState:
        //ui->recordButton->setEnabled(true);
        //ui->pauseButton->setEnabled(true);
        //ui->stopButton->setEnabled(false);
        //ui->metaDataButton->setEnabled(true);
        break;
    case QMediaRecorder::PausedState:
        //ui->recordButton->setEnabled(true);
        //ui->pauseButton->setEnabled(false);
        //ui->stopButton->setEnabled(true);
        //ui->metaDataButton->setEnabled(false);
        break;
    case QMediaRecorder::RecordingState:
        //ui->recordButton->setEnabled(false);
        //ui->pauseButton->setEnabled(true);
        //ui->stopButton->setEnabled(true);
        //ui->metaDataButton->setEnabled(false);
        break;
    }
}

void Camera::setExposureCompensation(int index)
{
    m_camera->setExposureCompensation(index * 0.5);
}

void Camera::displayRecorderError()
{
    if (m_mediaRecorder->error() != QMediaRecorder::NoError)
        QMessageBox::warning(this, tr("Capture Error"), m_mediaRecorder->errorString());
}

void Camera::displayCameraError()
{
    if (m_camera->error() != QCamera::NoError)
        QMessageBox::warning(this, tr("Camera Error"), m_camera->errorString());
}

void Camera::updateCameraDevice(QAction *action)
{
    setCamera(qvariant_cast<QCameraDevice>(action->data()));
}

void Camera::displayViewfinder()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void Camera::displayCapturedImage()
{
    ui->stackedWidget->setCurrentIndex(1);
}

QPushButton* lastClickedButton = nullptr;

void Camera::on_takeImageButton_clicked()
{
    lastClickedButton = ui->takeImageButton;
    ui->UserGuess->setText("Predicting Writer...");
    // Your existing code for this button click
}

void Camera::on_takeImageButton_2_clicked()
{
    lastClickedButton = ui->takeImageButton_2;
    ui->UserGuess->setText("Predicting Writers...");
    // Your existing code for this button click
}

void Camera::readyForCapture(bool ready)
{
    if (lastClickedButton) {
        lastClickedButton->setEnabled(ready);
    }
}

void Camera::imageSaved(int id, const QString &fileName)
{
    Q_UNUSED(id);

    QString customSaveLocation = "../camera/data/testcases/002/";
    QString specificFileName = "test_image.png";

    QString newFilePath = customSaveLocation + specificFileName;
    std::cout << "newFilePath: " << newFilePath.toStdString() << std::endl;
    QFile::remove(newFilePath);
    QFile::copy(fileName, newFilePath);

    QFile::remove(fileName);

    m_isCapturingImage = false;
    if (m_applicationExiting)
        close();

    string writername = "";
    if (lastClickedButton == ui->takeImageButton) {
        // Do something specific for takeImageButton
        IdentifyWriter writerIdentifier;
        writername = writerIdentifier.run("paragraph");
    } else if (lastClickedButton == ui->takeImageButton_2) {
        // Do something specific for takeImageButton_2
        IdentifyWriter writerIdentifier;
        writername = writerIdentifier.run("line");
    }

    lastClickedButton = nullptr; // Reset for the next round

    ui->UserGuess->setText(QString::fromStdString(writername));
}

void Camera::closeEvent(QCloseEvent *event)
{
    if (m_isCapturingImage) {
        setEnabled(false);
        m_applicationExiting = true;
        event->ignore();
    } else {
        event->accept();
    }
}

void Camera::updateCameras()
{
    ui->menuDevices->clear();
    const QList<QCameraDevice> availableCameras = QMediaDevices::videoInputs();
    for (const QCameraDevice &cameraDevice : availableCameras) {
        QAction *videoDeviceAction = new QAction(cameraDevice.description(), videoDevicesGroup);
        videoDeviceAction->setCheckable(true);
        videoDeviceAction->setData(QVariant::fromValue(cameraDevice));
        if (cameraDevice == QMediaDevices::defaultVideoInput())
            videoDeviceAction->setChecked(true);

        ui->menuDevices->addAction(videoDeviceAction);
    }
}

void Camera::showMetaDataDialog()
{
    if (!m_metaDataDialog)
        m_metaDataDialog = new MetaDataDialog(this);
    m_metaDataDialog->setAttribute(Qt::WA_DeleteOnClose, false);
    if (m_metaDataDialog->exec() == QDialog::Accepted)
        saveMetaData();
}

void Camera::saveMetaData()
{
    QMediaMetaData data;
    for (int i = 0; i < QMediaMetaData::NumMetaData; i++) {
        QString val = m_metaDataDialog->m_metaDataFields[i]->text();
        if (!val.isEmpty()) {
            auto key = static_cast<QMediaMetaData::Key>(i);
            if (i == QMediaMetaData::CoverArtImage) {
                QImage coverArt(val);
                data.insert(key, coverArt);
            } else if (i == QMediaMetaData::ThumbnailImage) {
                QImage thumbnail(val);
                data.insert(key, thumbnail);
            } else if (i == QMediaMetaData::Date) {
                QDateTime date = QDateTime::fromString(val);
                data.insert(key, date);
            } else {
                data.insert(key, val);
            }
        }
    }
    m_mediaRecorder->setMetaData(data);
}

void Camera::on_changeToDrawButton_clicked()
{
    ui->PageStackedWidget->setCurrentIndex(1);
    ui->TextInput->clear();
    ui->writingStatus->setText("Status");
    updateSerialPortList();

}


void Camera::on_changeToAnalyzeButton_clicked()
{
    ui->PageStackedWidget->setCurrentIndex(0);
}


void Camera::on_ClearButton_clicked()
{
    ui->TextInput->clear();
}

void Camera::on_SendButton_clicked()
{
    if(thread->isRunning()){
        QMessageBox::information(this, "Can not send data", "Please stop the current instructions");
        return;
    }

    int currentIndex = ui->comboBox->currentIndex();
    QVariant userData = ui->comboBox->itemData(currentIndex, Qt::UserRole);

    QString username = userData.toString();
    QString textInput = ui->TextInput->toPlainText();
    QString serialPort = ui->serialComboBox->currentText();

    if (textInput.length() >= 3)
    {
        if(serialPort.length() > 3)
        {
            // Create a Worker instance
            worker = new Worker(username, textInput, serialPort, ui->writingStatus);

            // Move the Worker object to the thread
            worker->moveToThread(thread);

            // Connect the thread's started signal to the doWork slot in the Worker object
            QObject::connect(thread, &QThread::started, worker, &Worker::CreateText);

            connect(worker, &Worker::updateStatus, ui->writingStatus, &QLabel::setText);
            connect(worker, &Worker::finished, this, &Camera::stop_ongoing_work);

            // Start the thread
            thread->start();
        }
        else
        {
            QMessageBox::information(this, "Can not send data", "Please choose a serial port.");
        }
    }
    else
    {
        QMessageBox::information(this, "Can not send data", "Input Text should be at least 3 characters long.");
    }
}

void Camera::on_StopButton_clicked()
{
    if(thread->isRunning()){
        worker->stopWork();
        QMessageBox::information(this, "Instructions Terminated", "Stoped current progress.");
        ui->writingStatus->setText("Stopped");
    }
}

void Camera::stop_ongoing_work()
{
    if(thread && worker) {
        if (thread->isRunning()) {
            thread->quit();
            thread->wait(); // Wait for the thread to finish

            // Reset the pointers to avoid dangling pointers
            delete worker; // Delete the worker object when it has finished its work
            delete thread; // Delete the thread object

            worker = nullptr;
            thread = nullptr;

            thread = new QThread();
        }
    }
}

QStringList Camera::listSerialPorts() {
    QStringList portList;

    Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
        QString qPortName = port.portName();
        portList.append(qPortName);
    }

    return portList;
}

void Camera::updateSerialPortList() {
    QStringList portList = listSerialPorts();
    ui->serialComboBox->clear();
    ui->serialComboBox->addItems(portList);
}

QStringList Camera::getUserProfiles() {
    QString directoryPath = QString::fromStdString(USER_PROFILE_PATH);

    QDir directory(directoryPath);
    QStringList folders;

    if (!directory.exists()) {
        qDebug() << "User Profile Directory does not exist.";
        return folders;
    }

    folders = directory.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    return folders;
}

void Camera::generateUserProfileList() {
    // Clear existing menu actions before regenerating
    QList<QAction*> actions = ui->menuDelete_Profile->actions();
    for (QAction* action : actions) {
        ui->menuDelete_Profile->removeAction(action);
        delete action;
    }

    QStringList userProfiles = getUserProfiles();

    for (const QString &folderName : userProfiles) {
        QAction *deleteAction = new QAction(folderName, this); // Create a new QAction with folderName as text
        ui->menuDelete_Profile->addAction(deleteAction); // Add the action to the menu

        QString displayText =  folderName;
        displayText[0] = displayText[0].toUpper();

        deleteAction->setText(displayText);

        // Connect the triggered signal of the action to a slot or lambda function
        connect(deleteAction, &QAction::triggered, [=]() {
            // Show a confirmation dialog before deletion
            QMessageBox::StandardButton confirmDelete;
            confirmDelete = QMessageBox::warning(this, "Confirm Deletion",
                                                 QString("Are you sure you want to delete user profile '%1'?").arg(folderName),
                                                 QMessageBox::Yes | QMessageBox::No);

            // If the user confirms deletion, proceed with deletion
            if (confirmDelete == QMessageBox::Yes) {
                QString folderPath = QString::fromStdString(USER_PROFILE_PATH);

                QString folderToDelete = QString::fromStdString(USER_PROFILE_PATH) + folderName;

                QDir dirToDelete(folderToDelete);
                bool success = dirToDelete.removeRecursively(); // Delete the folder recursively

                if (success) {
                    generateUserProfileList();
                    QMessageBox::information(this, "Deletion Successful", "Profile deleted successfully.");
                    // Update UI or perform any other actions after successful deletion
                } else {
                    QMessageBox::critical(this, "Deletion Error", "Failed to delete profile.");
                    // Handle deletion error if required
                }
            }
        });
    }
}

QStringList Camera::getUserFonts() {
    QString directoryPath = QString::fromStdString(USER_FONT_PATH);

    QDir directory(directoryPath);
    QStringList fontFiles;

    if (!directory.exists()) {
        qDebug() << "User Font Directory does not exist.";
        return fontFiles;
    }

    QStringList allFiles = directory.entryList(QDir::Files); // Get all files in the directory

    // Filter only .ttf files
    for (const QString &file : allFiles) {
        if (file.endsWith(".ttf", Qt::CaseInsensitive)) {
            fontFiles.append(file);
        }
    }

    return fontFiles;
}


void Camera::generateUserFontList() {
    // Clear existing menu actions before regenerating
    QList<QAction*> actions = ui->menuDelete_Font->actions();
    for (QAction* action : actions) {
        ui->menuDelete_Font->removeAction(action);
        delete action;
    }

    ui->comboBox->clear();

    QStringList userFonts = getUserFonts();


    int index = 0;
    for (const QString &fileName : userFonts) {
        QString displayText =  fileName.left(fileName.lastIndexOf("."));
        displayText[0] = displayText[0].toUpper();

        QAction *deleteAction = new QAction(fileName, this); // Create a new QAction with folderName as text
        ui->menuDelete_Font->addAction(deleteAction); // Add the action to the menu
        deleteAction->setText(displayText);

        QVariant dataValue(fileName);
        ui->comboBox->addItem(displayText);
        ui->comboBox->setItemData(index, dataValue, Qt::UserRole);

        index++;

        // Connect the triggered signal of the action to a slot or lambda function
        connect(deleteAction, &QAction::triggered, [=]() {
            // Show a confirmation dialog before deletion
            QMessageBox::StandardButton confirmDelete;
            confirmDelete = QMessageBox::warning(this, "Confirm Deletion",
                                                 QString("Are you sure you want to delete user font '%1'?").arg(fileName),
                                                 QMessageBox::Yes | QMessageBox::No);

            // If the user confirms deletion, proceed with deletion
            if (confirmDelete == QMessageBox::Yes) {
                QString filePath = QString::fromStdString(USER_FONT_PATH);

                QString fileToDelete = QString::fromStdString(USER_FONT_PATH) + fileName;

                QFile file(fileToDelete);
                bool success = file.remove();

                if (success) {
                    generateUserFontList();
                    QMessageBox::information(this, "Deletion Successful", "File deleted successfully.");
                    // Update UI or perform any other actions after successful deletion
                } else {
                    QMessageBox::critical(this, "Deletion Error", "Failed to delete file.");
                    // Handle deletion error if required
                }
            }
        });
    }
}


void Camera::on_actionAdd_Profile_triggered()
{
    ui->PageStackedWidget->setCurrentIndex(2);
}


void Camera::on_actionAdd_Font_triggered()
{
    ui->PageStackedWidget->setCurrentIndex(3);
}


void Camera::on_goBack_1_clicked()
{
    ui->PageStackedWidget->setCurrentIndex(0);
    ui->newProfileUsernameInput->setText("");
    ui->saveNewProfileImageButton->setText("Upload File");
    uploadedFilePaths.clear();

    fillListView();
}


void Camera::on_goBack_2_clicked()
{
    ui->PageStackedWidget->setCurrentIndex(0);
    ui->newUsernameInput->setText("");
    ui->saveNewFontImageButton->setText("Upload File");
    uploadedFilePath = "";
}

void Camera::on_saveNewFontImageButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath());
    if (!filePath.isEmpty()) {
        ui->saveNewFontImageButton->setText(filePath);
        uploadedFilePath = filePath;
    }
}

void Camera::on_saveNewFontButton_clicked(){
    QString textInput = ui->newUsernameInput->toPlainText();

    if(textInput.length() >= 3 && uploadedFilePath.length() != 0) {
        QStringList fileNames = getUserFonts();

        for(const QString &fileName : fileNames){
            QString username =  fileName.left(fileName.lastIndexOf("."));

            if(username == textInput) {
                QMessageBox::information(this, "Can not save new font", "Username already exists.");
                return;
            }
        }

        //
        // uploaded file info
        //
        QFileInfo fileInfo(uploadedFilePath);
        QString fileName = fileInfo.fileName();
        QString fileExtension = fileInfo.suffix();

        qDebug() << uploadedFilePath <<QString::fromStdString(USER_FONT_PATH) + fileName << fileName << fileExtension;

        if (fileExtension.toLower() == "jpg") {
            FontRenderer fontRenderer;
            //fontRenderer.scanImage(uploadedFilePath.toStdString(), uploadedFilePath.toStdString());

            std::string sheet = uploadedFilePath.toStdString();
            std::string output_directory = "../camera/data/ttf";
            std::string directory = "../camera/data/tmp";
            std::string config = "../camera/default.json";
            std::string filename;
            std::string family;
            std::string style;

            std::cout << sheet << std::endl;

            std::string metadata = "{\"filename\": \"" + filename + "\", \"family\": \"" + family + "\", \"style\": \"" + style + "\"}";
            int success = Converter::converters(sheet, output_directory, directory, config, metadata, textInput.toStdString());
            std::cout << success << std::endl;
            //
            //  Save the file to the ttf folder
            //
            if (!success)
            {
                QMessageBox::information(this, "Font Saved", "New font saved for the user `" + textInput + "`.");
                generateUserFontList();
                on_goBack_2_clicked();

            }
            else
            {
                QMessageBox::critical(this, "Error", "Failed to save font file.");
            }

        } else {
            QMessageBox::warning(this, "Invalid File", "Please select a JPG file.");
        }
        /*

        Turn the uploaded page into a ttf file

        */


    }
    else if (textInput.length() < 3){
        QMessageBox::information(this, "Can not save new font", "Username should be at least 3 characters long.");
    }
    else {
        QMessageBox::information(this, "Can not save new font", "Please upload the template file.");
    }
}


void Camera::on_saveNewProfileImageButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath());

    if (!filePath.isEmpty()) {
        QFileInfo fileInfo(filePath);
        QString fileExtension = fileInfo.suffix().toLower(); // Get the file extension in lowercase

        if (fileExtension == "png" || fileExtension == "jpg" || fileExtension == "jpeg") {
            ui->saveNewProfileImageButton->setText(filePath);
            uploadedFilePaths.append(filePath);
            fillListView();
        } else {
            QMessageBox::warning(this, "Invalid File", "Please select a PNG, JPG, or JPEG file.");
        }
    }
}


void Camera::fillListView() {
    ui->listWidget->clear();

    for (const QString& item : uploadedFilePaths) {
        // Create a QListWidgetItem with text
        QListWidgetItem* listItem = new QListWidgetItem(item, ui->listWidget);
        listItem->setBackground(QBrush(QColor(85, 170, 255)));

        QFontMetrics metrics(ui->listWidget->font());
        QSize size = metrics.size(Qt::TextSingleLine, item);
        size.setHeight(50);
        listItem->setSizeHint(size);

        // Create QLabel to display text
        QLabel* textLabel = new QLabel(item);
        textLabel->setStyleSheet("font-size: 20px;");
        textLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

        // Create a QPushButton with text "X" for removal
        QPushButton* removeButton = new QPushButton("X");
        removeButton->setStyleSheet("background-color: black; color: rgb(85, 170, 255); padding: 5px;");
        removeButton->setFont(QFont("Arial", 12));

        // Create a widget to hold the label and button
        QWidget* itemWidget = new QWidget(ui->listWidget);

        QVBoxLayout* mainLayout = new QVBoxLayout(itemWidget);
        QHBoxLayout* layout = new QHBoxLayout();
        layout->addWidget(textLabel);
        layout->addStretch();
        layout->addWidget(removeButton);

        mainLayout->addLayout(layout);
        mainLayout->setContentsMargins(10, 5, 10, 5);
        mainLayout->setSpacing(10);
        itemWidget->setLayout(mainLayout);

        // Set the item widget to the QListWidgetItem
        ui->listWidget->setItemWidget(listItem, itemWidget);

        connect(removeButton, &QPushButton::clicked, [listItem, this]() {
            int row = ui->listWidget->row(listItem);

            uploadedFilePaths.removeAt(row);
            delete listItem;

            fillListView();
        });
    }
}


void Camera::on_saveNewProfileButton_clicked()
{
    QString textInput = ui->newProfileUsernameInput->toPlainText();

    if(textInput.length() >= 3 && uploadedFilePaths.length() != 0) {
        QStringList userProfiles = getUserProfiles();

        for(const QString &username : userProfiles){
            if(username == textInput) {
                QMessageBox::information(this, "Can not save new profile", "Username already exists.");
                return;
            }
        }

        QString newProfileLocation = QString::fromStdString(USER_PROFILE_PATH) + textInput;
        QDir directory(newProfileLocation);
        directory.mkpath(".");


        int index = 1;
        for(const QString &uploadedImage : uploadedFilePaths){

            QString newFilePath = newProfileLocation + "/training_image_" + QString::number(index) + ".jpg";


            if (QFile::copy(uploadedImage, newFilePath))
            {
                // newFilePath;

                if(!IdentifyWriter().scanImage(uploadedImage.toStdString(), newFilePath.toStdString())) {
                    QMessageBox::critical(this, "Error", "Failed to save new profile.");
                    directory.removeRecursively();
                    return;
                }
            }
            else
            {
                QMessageBox::critical(this, "Error", "Failed to save new profile.");
                directory.removeRecursively();
                return;
            }

            index++;
        }


        QMessageBox::information(this, "Profile Saved", "New profile saved for the user `" + textInput + "`.");
        generateUserProfileList();
        on_goBack_1_clicked();
    }
    else if (textInput.length() < 3){
        QMessageBox::information(this, "Can not save new profile", "Username should be at least 3 characters long.");
    }
    else {
        QMessageBox::information(this, "Can not save new profile", "Please upload at least one file.");
    }
}


void Camera::on_refreshPorts_clicked()
{
    updateSerialPortList();
}
