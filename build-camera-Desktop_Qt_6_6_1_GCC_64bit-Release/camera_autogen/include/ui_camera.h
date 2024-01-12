/********************************************************************************
** Form generated from reading UI file 'camera.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAMERA_H
#define UI_CAMERA_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QIcon>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Camera
{
public:
    QAction *actionExit;
    QAction *actionStartCamera;
    QAction *actionStopCamera;
    QAction *actionAdd_Profile;
    QAction *actionAdd_Font;
    QWidget *centralwidget;
    QStackedWidget *PageStackedWidget;
    QWidget *Page1;
    QWidget *topbar;
    QLabel *PercentageGuess;
    QLabel *UserGuess;
    QPushButton *changeToDrawButton;
    QPushButton *takeImageButton;
    QPushButton *takeImageButton_2;
    QStackedWidget *stackedWidget;
    QWidget *viewfinderPage;
    QGridLayout *gridLayout_5;
    QVideoWidget *viewfinder;
    QWidget *previewPage;
    QGridLayout *gridLayout_4;
    QLabel *lastImagePreviewLabel;
    QWidget *Page2;
    QTextEdit *TextInput;
    QPushButton *SendButton;
    QPushButton *ClearButton;
    QPushButton *changeToAnalyzeButton;
    QPushButton *StopButton;
    QComboBox *comboBox;
    QComboBox *serialComboBox;
    QPushButton *refreshPorts;
    QLabel *writingStatus;
    QWidget *Page3;
    QPushButton *goBack_1;
    QTextEdit *newProfileUsernameInput;
    QLabel *newProfileNameLabel;
    QPushButton *saveNewProfileImageButton;
    QPushButton *saveNewProfileButton;
    QLabel *newProfileImageLabel;
    QListWidget *listWidget;
    QWidget *Page4;
    QPushButton *goBack_2;
    QTextEdit *newUsernameInput;
    QLabel *newFontImageLabel;
    QPushButton *saveNewFontButton;
    QLabel *newUsernameLabel;
    QPushButton *saveNewFontImageButton;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuDevices;
    QMenu *menuUser_Profiles;
    QMenu *menuDelete_Profile;
    QMenu *menuUser_Fonts;
    QMenu *menuDelete_Font;

    void setupUi(QMainWindow *Camera)
    {
        if (Camera->objectName().isEmpty())
            Camera->setObjectName("Camera");
        Camera->resize(1200, 781);
        actionExit = new QAction(Camera);
        actionExit->setObjectName("actionExit");
        actionStartCamera = new QAction(Camera);
        actionStartCamera->setObjectName("actionStartCamera");
        actionStopCamera = new QAction(Camera);
        actionStopCamera->setObjectName("actionStopCamera");
        actionAdd_Profile = new QAction(Camera);
        actionAdd_Profile->setObjectName("actionAdd_Profile");
        actionAdd_Font = new QAction(Camera);
        actionAdd_Font->setObjectName("actionAdd_Font");
        centralwidget = new QWidget(Camera);
        centralwidget->setObjectName("centralwidget");
        PageStackedWidget = new QStackedWidget(centralwidget);
        PageStackedWidget->setObjectName("PageStackedWidget");
        PageStackedWidget->setGeometry(QRect(0, 0, 1200, 760));
        PageStackedWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(20, 40, 90)"));
        Page1 = new QWidget();
        Page1->setObjectName("Page1");
        topbar = new QWidget(Page1);
        topbar->setObjectName("topbar");
        topbar->setGeometry(QRect(20, 10, 1161, 81));
        PercentageGuess = new QLabel(topbar);
        PercentageGuess->setObjectName("PercentageGuess");
        PercentageGuess->setGeometry(QRect(730, 10, 71, 61));
        QFont font;
        font.setPointSize(16);
        PercentageGuess->setFont(font);
        PercentageGuess->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 170, 255);border-radius: 7px"));
        PercentageGuess->setAlignment(Qt::AlignCenter);
        UserGuess = new QLabel(topbar);
        UserGuess->setObjectName("UserGuess");
        UserGuess->setGeometry(QRect(220, 10, 491, 61));
        QFont font1;
        font1.setPointSize(12);
        UserGuess->setFont(font1);
        UserGuess->setLayoutDirection(Qt::LeftToRight);
        UserGuess->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 170, 255);border-radius: 7px"));
        UserGuess->setAlignment(Qt::AlignCenter);
        changeToDrawButton = new QPushButton(topbar);
        changeToDrawButton->setObjectName("changeToDrawButton");
        changeToDrawButton->setGeometry(QRect(20, 10, 121, 61));
        QFont font2;
        font2.setPointSize(12);
        font2.setBold(true);
        changeToDrawButton->setFont(font2);
        changeToDrawButton->setCursor(QCursor(Qt::PointingHandCursor));
        changeToDrawButton->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 85, 127);\n"
"color: white;border-radius: 3px"));
        takeImageButton = new QPushButton(topbar);
        takeImageButton->setObjectName("takeImageButton");
        takeImageButton->setEnabled(true);
        takeImageButton->setGeometry(QRect(880, 10, 121, 61));
        QFont font3;
        font3.setPointSize(14);
        font3.setBold(false);
        takeImageButton->setFont(font3);
        takeImageButton->setCursor(QCursor(Qt::PointingHandCursor));
        takeImageButton->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 170, 255);border-radius: 3px; color: white;"));
        takeImageButton_2 = new QPushButton(topbar);
        takeImageButton_2->setObjectName("takeImageButton_2");
        takeImageButton_2->setEnabled(true);
        takeImageButton_2->setGeometry(QRect(1020, 10, 121, 61));
        QFont font4;
        font4.setPointSize(14);
        takeImageButton_2->setFont(font4);
        takeImageButton_2->setCursor(QCursor(Qt::PointingHandCursor));
        takeImageButton_2->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 170, 255);border-radius: 3px; color: white;"));
        stackedWidget = new QStackedWidget(Page1);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(30, 90, 1141, 651));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(stackedWidget->sizePolicy().hasHeightForWidth());
        stackedWidget->setSizePolicy(sizePolicy);
        QPalette palette;
        QBrush brush(QColor(20, 40, 90, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Active, QPalette::Window, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
        stackedWidget->setPalette(palette);
        viewfinderPage = new QWidget();
        viewfinderPage->setObjectName("viewfinderPage");
        gridLayout_5 = new QGridLayout(viewfinderPage);
        gridLayout_5->setObjectName("gridLayout_5");
        viewfinder = new QVideoWidget(viewfinderPage);
        viewfinder->setObjectName("viewfinder");
        viewfinder->setStyleSheet(QString::fromUtf8("background-color: rgb(12, 12, 12);"));

        gridLayout_5->addWidget(viewfinder, 0, 0, 1, 1);

        stackedWidget->addWidget(viewfinderPage);
        previewPage = new QWidget();
        previewPage->setObjectName("previewPage");
        gridLayout_4 = new QGridLayout(previewPage);
        gridLayout_4->setObjectName("gridLayout_4");
        lastImagePreviewLabel = new QLabel(previewPage);
        lastImagePreviewLabel->setObjectName("lastImagePreviewLabel");
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lastImagePreviewLabel->sizePolicy().hasHeightForWidth());
        lastImagePreviewLabel->setSizePolicy(sizePolicy1);
        lastImagePreviewLabel->setFrameShape(QFrame::Box);

        gridLayout_4->addWidget(lastImagePreviewLabel, 0, 0, 1, 1);

        stackedWidget->addWidget(previewPage);
        PageStackedWidget->addWidget(Page1);
        Page2 = new QWidget();
        Page2->setObjectName("Page2");
        TextInput = new QTextEdit(Page2);
        TextInput->setObjectName("TextInput");
        TextInput->setGeometry(QRect(240, 220, 521, 361));
        QFont font5;
        font5.setFamilies({QString::fromUtf8("Segoe UI")});
        font5.setPointSize(16);
        font5.setBold(false);
        TextInput->setFont(font5);
        TextInput->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 170, 255);\n"
"border-radius: 7px;\n"
"padding: 15px;"));
        SendButton = new QPushButton(Page2);
        SendButton->setObjectName("SendButton");
        SendButton->setGeometry(QRect(770, 340, 191, 121));
        SendButton->setFont(font);
        SendButton->setCursor(QCursor(Qt::PointingHandCursor));
        SendButton->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 170, 255);border-radius: 7px"));
        ClearButton = new QPushButton(Page2);
        ClearButton->setObjectName("ClearButton");
        ClearButton->setGeometry(QRect(770, 530, 191, 51));
        ClearButton->setFont(font);
        ClearButton->setCursor(QCursor(Qt::PointingHandCursor));
        ClearButton->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 170, 255);\n"
"border-radius: 7px;"));
        changeToAnalyzeButton = new QPushButton(Page2);
        changeToAnalyzeButton->setObjectName("changeToAnalyzeButton");
        changeToAnalyzeButton->setGeometry(QRect(40, 20, 121, 61));
        QFont font6;
        font6.setPointSize(12);
        font6.setBold(true);
        font6.setItalic(false);
        font6.setStrikeOut(false);
        font6.setKerning(true);
        changeToAnalyzeButton->setFont(font6);
        changeToAnalyzeButton->setCursor(QCursor(Qt::PointingHandCursor));
        changeToAnalyzeButton->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 85, 127);\n"
"color: white;border-radius: 3px"));
        StopButton = new QPushButton(Page2);
        StopButton->setObjectName("StopButton");
        StopButton->setGeometry(QRect(770, 470, 191, 51));
        StopButton->setFont(font);
        StopButton->setCursor(QCursor(Qt::PointingHandCursor));
        StopButton->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 170, 255);\n"
"border-radius: 7px;"));
        comboBox = new QComboBox(Page2);
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(770, 220, 191, 51));
        comboBox->setFont(font);
        comboBox->setStyleSheet(QString::fromUtf8("background-color: rgba(85, 170, 255, 177);\n"
"border-radius: 7px;\n"
"padding: 15px;"));
        serialComboBox = new QComboBox(Page2);
        serialComboBox->setObjectName("serialComboBox");
        serialComboBox->setGeometry(QRect(770, 280, 191, 51));
        serialComboBox->setFont(font);
        serialComboBox->setStyleSheet(QString::fromUtf8("background-color: rgba(85, 170, 255, 177);\n"
"border-radius: 7px;\n"
"padding: 15px;"));
        refreshPorts = new QPushButton(Page2);
        refreshPorts->setObjectName("refreshPorts");
        refreshPorts->setGeometry(QRect(970, 280, 51, 51));
        refreshPorts->setFont(font);
        refreshPorts->setCursor(QCursor(Qt::PointingHandCursor));
        refreshPorts->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 170, 255);\n"
"border-radius: 7px;"));
        QIcon icon;
        icon.addFile(QString::fromUtf8("../images/refresh.svg"), QSize(), QIcon::Normal, QIcon::Off);
        refreshPorts->setIcon(icon);
        refreshPorts->setIconSize(QSize(32, 32));
        refreshPorts->setCheckable(false);
        writingStatus = new QLabel(Page2);
        writingStatus->setObjectName("writingStatus");
        writingStatus->setGeometry(QRect(370, 130, 491, 61));
        writingStatus->setFont(font);
        writingStatus->setLayoutDirection(Qt::LeftToRight);
        writingStatus->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 170, 255);border-radius: 7px"));
        writingStatus->setAlignment(Qt::AlignCenter);
        PageStackedWidget->addWidget(Page2);
        Page3 = new QWidget();
        Page3->setObjectName("Page3");
        goBack_1 = new QPushButton(Page3);
        goBack_1->setObjectName("goBack_1");
        goBack_1->setGeometry(QRect(40, 20, 121, 61));
        goBack_1->setFont(font2);
        goBack_1->setCursor(QCursor(Qt::PointingHandCursor));
        goBack_1->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 85, 127);\n"
"color: white;border-radius: 3px"));
        newProfileUsernameInput = new QTextEdit(Page3);
        newProfileUsernameInput->setObjectName("newProfileUsernameInput");
        newProfileUsernameInput->setGeometry(QRect(70, 250, 461, 71));
        newProfileUsernameInput->setFont(font5);
        newProfileUsernameInput->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 170, 255);\n"
"border-radius: 7px;\n"
"padding: 15px;"));
        newProfileNameLabel = new QLabel(Page3);
        newProfileNameLabel->setObjectName("newProfileNameLabel");
        newProfileNameLabel->setGeometry(QRect(70, 180, 141, 61));
        QFont font7;
        font7.setPointSize(20);
        newProfileNameLabel->setFont(font7);
        newProfileNameLabel->setLayoutDirection(Qt::LeftToRight);
        newProfileNameLabel->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 170, 255);border-radius: 7px"));
        newProfileNameLabel->setAlignment(Qt::AlignCenter);
        saveNewProfileImageButton = new QPushButton(Page3);
        saveNewProfileImageButton->setObjectName("saveNewProfileImageButton");
        saveNewProfileImageButton->setGeometry(QRect(70, 430, 461, 71));
        saveNewProfileImageButton->setFont(font);
        saveNewProfileImageButton->setCursor(QCursor(Qt::PointingHandCursor));
        saveNewProfileImageButton->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 170, 255);\n"
"border-radius: 7px;"));
        saveNewProfileButton = new QPushButton(Page3);
        saveNewProfileButton->setObjectName("saveNewProfileButton");
        saveNewProfileButton->setGeometry(QRect(210, 580, 191, 51));
        saveNewProfileButton->setFont(font);
        saveNewProfileButton->setCursor(QCursor(Qt::PointingHandCursor));
        saveNewProfileButton->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 170, 255);\n"
"border-radius: 7px;"));
        newProfileImageLabel = new QLabel(Page3);
        newProfileImageLabel->setObjectName("newProfileImageLabel");
        newProfileImageLabel->setGeometry(QRect(70, 360, 241, 61));
        newProfileImageLabel->setFont(font7);
        newProfileImageLabel->setLayoutDirection(Qt::LeftToRight);
        newProfileImageLabel->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 170, 255);border-radius: 7px"));
        newProfileImageLabel->setAlignment(Qt::AlignCenter);
        listWidget = new QListWidget(Page3);
        listWidget->setObjectName("listWidget");
        listWidget->setGeometry(QRect(590, 60, 551, 651));
        listWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 170, 255);\n"
"border-radius: 6px;"));
        PageStackedWidget->addWidget(Page3);
        Page4 = new QWidget();
        Page4->setObjectName("Page4");
        goBack_2 = new QPushButton(Page4);
        goBack_2->setObjectName("goBack_2");
        goBack_2->setGeometry(QRect(40, 20, 121, 61));
        goBack_2->setFont(font2);
        goBack_2->setCursor(QCursor(Qt::PointingHandCursor));
        goBack_2->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 85, 127);\n"
"color: white;border-radius: 3px"));
        newUsernameInput = new QTextEdit(Page4);
        newUsernameInput->setObjectName("newUsernameInput");
        newUsernameInput->setGeometry(QRect(190, 230, 821, 71));
        newUsernameInput->setFont(font5);
        newUsernameInput->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 170, 255);\n"
"border-radius: 7px;\n"
"padding: 15px;"));
        newFontImageLabel = new QLabel(Page4);
        newFontImageLabel->setObjectName("newFontImageLabel");
        newFontImageLabel->setGeometry(QRect(190, 160, 151, 61));
        newFontImageLabel->setFont(font7);
        newFontImageLabel->setLayoutDirection(Qt::LeftToRight);
        newFontImageLabel->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 170, 255);border-radius: 7px"));
        newFontImageLabel->setAlignment(Qt::AlignCenter);
        saveNewFontButton = new QPushButton(Page4);
        saveNewFontButton->setObjectName("saveNewFontButton");
        saveNewFontButton->setGeometry(QRect(510, 570, 191, 51));
        saveNewFontButton->setFont(font);
        saveNewFontButton->setCursor(QCursor(Qt::PointingHandCursor));
        saveNewFontButton->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 170, 255);\n"
"border-radius: 7px;"));
        newUsernameLabel = new QLabel(Page4);
        newUsernameLabel->setObjectName("newUsernameLabel");
        newUsernameLabel->setGeometry(QRect(190, 360, 151, 61));
        newUsernameLabel->setFont(font7);
        newUsernameLabel->setLayoutDirection(Qt::LeftToRight);
        newUsernameLabel->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 170, 255);border-radius: 7px"));
        newUsernameLabel->setAlignment(Qt::AlignCenter);
        saveNewFontImageButton = new QPushButton(Page4);
        saveNewFontImageButton->setObjectName("saveNewFontImageButton");
        saveNewFontImageButton->setGeometry(QRect(190, 430, 821, 71));
        saveNewFontImageButton->setFont(font);
        saveNewFontImageButton->setCursor(QCursor(Qt::PointingHandCursor));
        saveNewFontImageButton->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 170, 255);\n"
"border-radius: 7px;"));
        PageStackedWidget->addWidget(Page4);
        Camera->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Camera);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1200, 21));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        menuDevices = new QMenu(menubar);
        menuDevices->setObjectName("menuDevices");
        menuUser_Profiles = new QMenu(menubar);
        menuUser_Profiles->setObjectName("menuUser_Profiles");
        menuDelete_Profile = new QMenu(menuUser_Profiles);
        menuDelete_Profile->setObjectName("menuDelete_Profile");
        menuUser_Fonts = new QMenu(menubar);
        menuUser_Fonts->setObjectName("menuUser_Fonts");
        menuDelete_Font = new QMenu(menuUser_Fonts);
        menuDelete_Font->setObjectName("menuDelete_Font");
        Camera->setMenuBar(menubar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuDevices->menuAction());
        menubar->addAction(menuUser_Profiles->menuAction());
        menubar->addAction(menuUser_Fonts->menuAction());
        menuFile->addAction(actionStartCamera);
        menuFile->addAction(actionStopCamera);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuUser_Profiles->addAction(actionAdd_Profile);
        menuUser_Profiles->addAction(menuDelete_Profile->menuAction());
        menuUser_Fonts->addAction(actionAdd_Font);
        menuUser_Fonts->addAction(menuDelete_Font->menuAction());

        retranslateUi(Camera);
        QObject::connect(actionExit, &QAction::triggered, Camera, qOverload<>(&QMainWindow::close));
        QObject::connect(takeImageButton, SIGNAL(clicked()), Camera, SLOT(takeImage()));
        QObject::connect(actionStartCamera, SIGNAL(triggered()), Camera, SLOT(startCamera()));
        QObject::connect(actionStopCamera, SIGNAL(triggered()), Camera, SLOT(stopCamera()));
        QObject::connect(takeImageButton_2, SIGNAL(clicked()), Camera, SLOT(takeImage()));

        PageStackedWidget->setCurrentIndex(0);
        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Camera);
    } // setupUi

    void retranslateUi(QMainWindow *Camera)
    {
        Camera->setWindowTitle(QCoreApplication::translate("Camera", "Camera", nullptr));
        actionExit->setText(QCoreApplication::translate("Camera", "Close", nullptr));
        actionStartCamera->setText(QCoreApplication::translate("Camera", "Start Camera", nullptr));
        actionStopCamera->setText(QCoreApplication::translate("Camera", "Stop Camera", nullptr));
        actionAdd_Profile->setText(QCoreApplication::translate("Camera", "Add Profile", nullptr));
        actionAdd_Font->setText(QCoreApplication::translate("Camera", "Add Font", nullptr));
        PercentageGuess->setText(QCoreApplication::translate("Camera", "%", nullptr));
        UserGuess->setText(QString());
        changeToDrawButton->setText(QCoreApplication::translate("Camera", "Change to \n"
"Draw Menu", nullptr));
        takeImageButton->setText(QCoreApplication::translate("Camera", "Process \n"
"Paragraph", nullptr));
        takeImageButton_2->setText(QCoreApplication::translate("Camera", "Process \n"
"Lines", nullptr));
        lastImagePreviewLabel->setText(QString());
        SendButton->setText(QCoreApplication::translate("Camera", "Send To Machine", nullptr));
        ClearButton->setText(QCoreApplication::translate("Camera", "Clear", nullptr));
        changeToAnalyzeButton->setText(QCoreApplication::translate("Camera", "Change to \n"
"Analyze Menu", nullptr));
        StopButton->setText(QCoreApplication::translate("Camera", "Stop", nullptr));
        serialComboBox->setPlaceholderText(QCoreApplication::translate("Camera", "Serial Port", nullptr));
        refreshPorts->setText(QString());
        writingStatus->setText(QCoreApplication::translate("Camera", "Status", nullptr));
        goBack_1->setText(QCoreApplication::translate("Camera", "Go Back", nullptr));
        newProfileNameLabel->setText(QCoreApplication::translate("Camera", "Username", nullptr));
        saveNewProfileImageButton->setText(QCoreApplication::translate("Camera", "Upload File", nullptr));
        saveNewProfileButton->setText(QCoreApplication::translate("Camera", "Save Profile", nullptr));
        newProfileImageLabel->setText(QCoreApplication::translate("Camera", "Paragraph Image", nullptr));
        goBack_2->setText(QCoreApplication::translate("Camera", "Go Back", nullptr));
        newFontImageLabel->setText(QCoreApplication::translate("Camera", "Username", nullptr));
        saveNewFontButton->setText(QCoreApplication::translate("Camera", "Save Font", nullptr));
        newUsernameLabel->setText(QCoreApplication::translate("Camera", "Font Image", nullptr));
        saveNewFontImageButton->setText(QCoreApplication::translate("Camera", "Upload File", nullptr));
        menuFile->setTitle(QCoreApplication::translate("Camera", "File", nullptr));
        menuDevices->setTitle(QCoreApplication::translate("Camera", "Devices", nullptr));
        menuUser_Profiles->setTitle(QCoreApplication::translate("Camera", "User Profiles", nullptr));
        menuDelete_Profile->setTitle(QCoreApplication::translate("Camera", "Delete Profile", nullptr));
        menuUser_Fonts->setTitle(QCoreApplication::translate("Camera", "User Fonts", nullptr));
        menuDelete_Font->setTitle(QCoreApplication::translate("Camera", "Delete Font", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Camera: public Ui_Camera {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAMERA_H
