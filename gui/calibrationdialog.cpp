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

void CalibrationDialog::setManager(QScanManager *m)
{
    manager = m;

    printf("CalibrationDialog manager\t0x%x\n"
           "manager->getFirst()\t\t0x%x\n"
           "manager->getSecond()\t0x%x\n",
           manager,manager->getFirst(),
           manager->getSecond());

    if(manager->numCameras()<1){
        QMessageBox::warning(this,"No cameras registered",
                             "You must have at least one camera registered. "
                             "Please go to Camera Settings and select a camera.");
        close();
    }


    ui->previewFirst->setCamera(manager->getFirst());
    ui->previewFirst->startCameraStream();

    calibrator->setScanManager(manager);

    if(!(manager->isStereo())) return;

    ui->previewSecond->setCamera(manager->getSecond());
    ui->previewSecond->startCameraStream();

}

CalibrationDialog::~CalibrationDialog()
{
    delete ui;
}

void CalibrationDialog::takeSnap()
{
    cv::Mat left, right;
    int n;
    left = manager->getFirst()->getFrame();

    if(manager->isStereo()){
        right = manager->getSecond()->getFrame();
        n = calibrator->addImagePair(left,right);
    }else{
        n = calibrator->addSingleImage(left);
    }

    if(n>=0)
        ui->imageCount->setText(QString::number(n));
}

void CalibrationDialog::calibrate()
{
    if(manager->isStereo())
        calibrator->runCalibStereo();
    else
        calibrator->runCalibMono();
    reset();
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
