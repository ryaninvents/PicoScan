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
    connect(calib,
            SIGNAL(debug(QString)),
            this,
            SLOT(emitDebug(QString)));

}


CalibrationDialog::~CalibrationDialog()
{
    delete ui;
}

void CalibrationDialog::takeSnap()
{
    stopStreaming();
    calib->takeStereoFrame();
}

void CalibrationDialog::calibrate()
{
    calib->calibrate();
    close();
}

void CalibrationDialog::reset()
{
    calib->removeFrames();
    startStreaming();
}

void CalibrationDialog::close()
{
    calib->setEnabled(false);
    stopStreaming();
    QDialog::close();
}

void CalibrationDialog::calibrateProjector()
{
    stopStreaming();
    calib->takeBinaryFrame();
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
    calib->setEnabled(true);
    QDialog::show();
    startStreaming();
}

void CalibrationDialog::stopStreaming()
{
    ui->previewFirst->stopCameraStream();
    ui->previewSecond->stopCameraStream();
}

void CalibrationDialog::startStreaming()
{
    ui->previewFirst->startCameraStream();
    ui->previewSecond->startCameraStream();
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

void CalibrationDialog::setBinary(BinaryCompiler *binary)
{
    calib->setBinary(binary);
    connect(
                binary,
                SIGNAL(binaryFrameCaptured(cv::Mat,bool)),
                this,
                SLOT(binaryFrameCaptured(cv::Mat,bool)));
}

void CalibrationDialog::binaryFrameCaptured(cv::Mat, bool)
{
    startStreaming();
}

void CalibrationDialog::emitDebug(QString info)
{
    emit debug(info);
}

void CalibrationDialog::setCounter(uint ctr)
{
//    ui->imageCount->setText(QString::number(ctr));
    ui->lcdNumber->display((int)ctr);
    startStreaming();
}
