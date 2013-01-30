#include "calibrationdialog.h"
#include "ui_calibrationdialog.h"
#include <QMessageBox>

#include <opencv2/core/core.hpp>

CalibrationDialog::CalibrationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalibrationDialog)
{
    ui->setupUi(this);
    inProgress = new CalibrationInProgress;
}

void CalibrationDialog::setManager(ScanManager *m)
{
    manager = m;
    if(manager->numCameras()<2){
        QMessageBox::warning(this,"Not enough cameras","You must have at least two cameras attached to the system.");
        close();
    }
    ui->previewLeft->setCamera(manager->getLeft());
    ui->previewRight->setCamera(manager->getRight());

    ui->previewLeft->startCameraStream();
    ui->previewRight->startCameraStream();

    calibrator = new Calibrator(inProgress,manager);
}

CalibrationDialog::~CalibrationDialog()
{
    delete ui;
}

void CalibrationDialog::takeSnap()
{
    cv::Mat left, right;
    int n;
    left = manager->getLeft()->getFrame();
    right = manager->getRight()->getFrame();
    n = calibrator->addImagePair(left,right);
    if(n>=0)
        ui->imageCount->setText(QString::number(n));
}

void CalibrationDialog::calibrate()
{
    calibrator->runCalib();
    close();
}

void CalibrationDialog::reset()
{
    calibrator->clearFrames();
    ui->imageCount->setText(QString::number(0));
}
