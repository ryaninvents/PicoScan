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
    if(manager->numCameras()<1){
        QMessageBox::warning(this,"No cameras registered",
                             "You must have at least one camera registered. "
                             "Please go to Camera Settings and select a camera.");
        close();
    }

    calibrator = new Calibrator(inProgress,manager);

    ui->previewFirst->setCamera(manager->getFirst());
    ui->previewFirst->startCameraStream();

    if(!manager->isStereo()) return;

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
