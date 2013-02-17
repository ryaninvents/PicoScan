#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <stdio.h>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    calib(new CalibrationDialog)
{
    ui->setupUi(this);

    ui->debug->setPlainText(QString("ScanStudio started."));

    connect(&stdSettings,SIGNAL(accept()),this,SLOT(adjustCalStd()));

}

MainWindow::~MainWindow()
{
    delete ui;
    QCoreApplication::exit(0);

}

void MainWindow::debug(QString str)
{
    ui->debug->setPlainText(QString("%1\n%2")
                            .arg(ui->debug->toPlainText())
                            .arg(str));
    QScrollBar *sb = ui->debug->verticalScrollBar();
    sb->setValue(sb->maximum());
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
    enableCalibrate();
}

void MainWindow::showCalStdSettings()
{
    debug("Show calibration standard settings.");
    stdSettings.show();
}

void MainWindow::showCalibrationDialog()
{
    calib->show();
}

void MainWindow::quitProgram()
{
    QCoreApplication::exit(0);
}

void MainWindow::adjustCalStd()
{
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
