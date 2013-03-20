#include "calibrationdialog.h"
#include "ui_calibrationdialog.h"
#include <QMessageBox>

#include <opencv2/core/core.hpp>
#include <stdio.h>

#include "hardware/standards/dotmatrixstandard.h"

CalibrationDialog::CalibrationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalibrationDialog),
    calib(new CalibrationCompiler)
{
    ui->setupUi(this);

    standard = new DotMatrixStandard(cv::Size(9,17),
                                     17e-3, 8.5e-3,
                                     8.5e-3);
    calib->setStandard(standard);

    connect(calib,
            SIGNAL(framesCaptured(uint)),
            this,
            SLOT(setCounter(uint)));

}


CalibrationDialog::~CalibrationDialog()
{
    delete ui;
}

void CalibrationDialog::takeSnap()
{
    calib->takeStereoFrame();
}

void CalibrationDialog::calibrate()
{
    close();
}

void CalibrationDialog::reset()
{
    calib->removeFrames();
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

void CalibrationDialog::setLeft(QCamera *cam)
{
    left = cam;
    calib->setLeft(cam);
    ui->previewFirst->setCamera(cam);
    ui->previewFirst->startCameraStream();
}

void CalibrationDialog::show()
{
    QDialog::show();
    ui->previewFirst->startCameraStream();
}

void CalibrationDialog::frameCaptured(cv::Mat frame, QCamera *cam, QProjector::Pattern *pattern)
{
}

void CalibrationDialog::setRight(QCamera *cam)
{
    right = cam;
    calib->setRight(cam);
    ui->previewSecond->setCamera(cam);
    ui->previewSecond->startCameraStream();
}

void CalibrationDialog::setProjector(QProjector *proj)
{
    calib->setProjector(proj);
}

void CalibrationDialog::setCounter(uint ctr)
{
    ui->imageCount->setText(QString::number(ctr));
}
