#include <QFileDialog>
#include <QDebug>
#include <QQueue>
#include <QTextStream>
#include <QProcess>

#include <stdio.h>
#include <ios>
#include <iostream>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->choiceButton, SIGNAL(clicked()), SLOT(selectFileOrDir()));
    connect(ui->useDirCheckBox, SIGNAL(clicked(bool)), ui->typesList, SLOT(setEnabled(bool)));
    connect(ui->useDirCheckBox, SIGNAL(clicked(bool)), ui->usingTypesLabel, SLOT(setEnabled(bool)));
    loadSupportedEncodings();
    connect(ui->startButton, SIGNAL(clicked()), SLOT(startDecode()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::selectFileOrDir()
{
    QFileDialog dialog;
    if (ui->useDirCheckBox->isChecked()) {
        QStringList typesList = ui->typesList->text().split(", ");
        qDebug() << typesList;
        QString dir = dialog.getExistingDirectory();
        ui->filedir->setText(dir);
        QQueue<QString> queue;
        queue.append(dir);
        while (!queue.isEmpty()) {
            QDir currentDir(queue.dequeue());
            QStringList list = currentDir.entryList();
            foreach (QString i, list) {
                if (i[0] == '.') {
                    continue;
                }
                QString fullLink;
                fullLink.append(currentDir.absolutePath());
                fullLink.append("/");
                fullLink.append(i);
                QFileInfo info(fullLink);
                if (info.isDir()) {
                    queue.enqueue(fullLink);
                } else if (info.isFile()) {
                    if (typesList.size() > 0) {
                        foreach (QString type, typesList) {
                            if (fullLink.endsWith(type)) {
                                decoder.addFileLink(fullLink);
                                break;
                            }
                        }
                    }
                    else {
                        decoder.addFileLink(fullLink);
                    }
                }
            }
        }
    } else {
        foreach (QString link, dialog.getOpenFileNames())
        {
            ui->filedir->setText(link);
            decoder.addFileLink(link);
        }
    }
}

void MainWindow::startDecode()
{
    decoder.start(ui->inputEncoding->currentText(), ui->outputEncoding->currentText(), ui->saveOldCheckBox->isChecked());
    decoder.clear();
    ui->filedir->clear();
}

void MainWindow::loadSupportedEncodings()
{
    QProcess process;
    process.start("iconv --list");
    process.waitForFinished();

    QString standartOutput = process.readAllStandardOutput();
    QStringList encodingList = standartOutput.split("//\n");
    ui->inputEncoding->addItems(encodingList);
    ui->outputEncoding->addItems(encodingList);

    ui->inputEncoding->setCurrentText("WINDOWS-1251");
    ui->outputEncoding->setCurrentText("UTF-8");

    process.close();
}
