#include <iostream>
#include <QProcess>
#include <QString>
#include <QDebug>

#include "svgtottf.hpp"
using namespace std;

int SVGtoTTF::convert(const std::string& characters_dir, const std::string& output_directory, const std::string& config, const std::string& metadata, const std::string& stylename) {
    cout << "fontname" << stylename << endl;
    QProcess process;
    const QString scriptPath = "../camera/svgtottf.py";
    QString argument = QString::fromStdString(characters_dir);
    QString argument2 = QString::fromStdString(output_directory);
    QString argument3 = QString::fromStdString(config);
    QString argument4 = QString::fromStdString(metadata);
    QString argument5 = QString::fromStdString(stylename);

    QStringList command;
    command << scriptPath << argument << argument2 << argument3 << argument4 << argument5;

    QString joinedString = command.join(" ");
    std::string stdString = joinedString.toStdString();
    cout << stdString << endl;

    process.setProcessChannelMode(QProcess::MergedChannels);
    process.start("python3", command);
    if (process.waitForStarted()) {
        qDebug() << "Process started successfully.";
    } else {
        qDebug() << "Error starting the process:" << process.errorString();
    }

    QByteArray output = process.readAllStandardOutput();
    qDebug() << "Python Script Output: " << output;

    QObject::connect(&process, &QProcess::readyReadStandardOutput, [&]() {
        // Read the standard output of the process
        QByteArray output = process.readAllStandardOutput();

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
    if (process.exitCode() == 0) {
        qDebug() << "Python script executed successfully.";
    } else {
        qDebug() << "Error executing Python script. Exit code:" << process.exitCode();
        qDebug() << "Error details:" << process.errorString();
        qDebug() << "Standard Error Output:" << process.readAllStandardError();
    }

    return process.exitCode();
}
