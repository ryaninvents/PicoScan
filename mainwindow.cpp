#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <stdio.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    hardware(new HardwareManager),
    screen(new ProjectionScreen),
    manager(new ScanManager),
    calib(new CalibrationDialog),
    showingScreen(false),
    binBit(0),
    binInv(false),
    binaryMode(true)
{
    ui->setupUi(this);

    ui->debug->setPlainText(QString("ScanStudio started."));

    debug("ScanManager created.");

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

void MainWindow::debug(QString str)
{
    ui->debug->setPlainText(QString("%1\n%2")
                            .arg(ui->debug->toPlainText())
                            .arg(str));
}

void MainWindow::debug(const char *str)
{
    debug(QString(str));
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
    debug("Showing camera settings.");
    camSettings.show();
    camSettings.setScanManager(manager);
    enableCalibrate();
}

void MainWindow::showCalStdSettings()
{
    debug("Show calibration standard settings.");
    stdSettings.show();
}

void MainWindow::showProjectionScreen()
{
    if(!showingScreen){
        screen->show();
        showingScreen = true;

        screen->projectBinary(0,false,255);
        screen->projectOnDisplay(1);
    }else if(binaryMode){
        if(!binInv){
            screen->projectBinary(binBit,binInv,255);
            binInv = true;
        }else{
            if(binBit>=12){
                binBit = 0;
                binaryMode = false;
                binBit++;
                binInv = false;
            }else{
                screen->projectBinary(binBit,binInv,255);
                binBit++;
                binInv = false;
            }
        }
    }
    if(!binaryMode){
        screen->projectSinusoid(8,binBit,255);
        binBit++;
        if(binBit>8){
            binBit = 0;
            binInv = false;
            binaryMode = true;
        }
    }
    manager->setScreen(screen);
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
    manager->takeBinaryFrame();
}

void MainWindow::closeEvent(QCloseEvent *)
{
    quitProgram();
}

void MainWindow::enableCalibrate()
{
    ui->actionCalibrate->setEnabled(true);
}
