#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    manager = new ScanManager;
    //camSettings.setScanManager(manager);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showAbout()
{
    about.show();
}

void MainWindow::setFullScreen(bool fs)
{
    if(fs){
        showFullScreen();
    }else{
        showNormal();
    }
}

void MainWindow::showCameraSettings()
{
    camSettings.show();
}

void MainWindow::showCalStdSettings()
{
    stdSettings.show();
}
