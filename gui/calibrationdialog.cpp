#include "calibrationdialog.h"
#include "ui_calibrationdialog.h"
#include <QMessageBox>

#include <opencv2/core/core.hpp>
#include <stdio.h>

CalibrationDialog::CalibrationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalibrationDialog),
    inProgress(new CalibrationInProgress),
    calibrator(new Calibrator)
{
    ui->setupUi(this);
}


CalibrationDialog::~CalibrationDialog()
{
    delete ui;
}

void CalibrationDialog::takeSnap()
{
    /*cv::Mat left, right;
    int n;/*
    left = manager->getFirst()->getFrame();

    if(manager->isStereo()){
        right = manager->getSecond()->getFrame();
        n = calibrator->addImagePair(left,right);
    }else{
        n = calibrator->addSingleImage(left);
    }

    if(n>=0)
        ui->imageCount->setText(QString::number(n));*/
}

void CalibrationDialog::calibrate()
{/*
    if(manager->isStereo())
        calibrator->runCalibStereo();
    else
        calibrator->runCalibMono();
    reset();*/
    close();
}

void CalibrationDialog::reset()
{
    calibrator->clearFrames();
    ui->imageCount->setText(QString::number(0));
}

void CalibrationDialog::close()
{
    ui->previewFirst->closeCameraStream();
    ui->previewSecond->closeCameraStream();
    QDialog::close();
}

void CalibrationDialog::calibrateProjector()
{

}
