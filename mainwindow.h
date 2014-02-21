#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>

#include "decoder.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow * ui;
    Decoder decoder;

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void selectFileOrDir();
    void startDecode();
    void loadSupportedEncodings();
};

#endif // MAINWINDOW_H
