#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    hardware = new HardwareManager;
    camSettings.setHardwareManager(hardware);
    stdSettings.setManager(manager);
    screen = new ProjectionScreen;
    manager = new ScanManager;
    calib = new CalibrationDialog;

    connect(&stdSettings,SIGNAL(accept()),this,SLOT(adjustCalStd()));

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
    screen->projectOnDisplay(1);
}

void MainWindow::showCalibrationDialog()
{
    calib->show();
    calib->setManager(manager);
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
