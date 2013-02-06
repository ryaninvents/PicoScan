#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <stdio.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    hardware(new HardwareManager),
    screen(new ProjectionScreen),
    manager(new ScanManager),
    calib(new CalibrationDialog)
{
    ui->setupUi(this);

    printf("MainWindow manager\t\t0x%x\n",manager);

    connect(&stdSettings,SIGNAL(accept()),this,SLOT(adjustCalStd()));

    camSettings.setHardwareManager(hardware);
    camSettings.setScanManager(manager);

    stdSettings.setManager(manager);

    hardware->refreshCameras();

}

MainWindow::~MainWindow()
{
    delete screen;
    delete ui;
    QCoreApplication::exit(0);

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
    camSettings.setScanManager(manager);
    enableCalibrate();
}

void MainWindow::showCalStdSettings()
{
    stdSettings.show();
}

void MainWindow::showProjectionScreen()
{
    screen->show();

    screen->projectBinary(0,false);
    screen->projectOnDisplay(1);
}

void MainWindow::showCalibrationDialog()
{
    calib->setManager(manager);
    calib->show();
}

void MainWindow::quitProgram()
{
    QCoreApplication::exit(0);
}

void MainWindow::adjustCalStd()
{
    manager->setStandard(stdSettings.getStandard());
    enableCalibrate();
}

void MainWindow::takeFrame()
{
}

void MainWindow::closeEvent(QCloseEvent *)
{
    quitProgram();
}

void MainWindow::enableCalibrate()
{
    ui->actionCalibrate->setEnabled(true);
}
