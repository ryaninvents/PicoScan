#include "singlecalibrationdialog.h"
#include "ui_singlecalibrationdialog.h"

#include <iostream>
#include <opencv2/calib3d/calib3d.hpp>

SingleCalibrationDialog::SingleCalibrationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SingleCalibrationDialog),
    imagePoints()
{
    ui->setupUi(this);
}

SingleCalibrationDialog::~SingleCalibrationDialog()
{
    delete ui;
}

void SingleCalibrationDialog::setStandard(CalibrationStandard *standard)
{
    this->standard = standard;
    imagePoints.clear();
    ui->lcdNumber->display((int)0);
}

void SingleCalibrationDialog::setCamera(QCamera *camera)
{
    this->camera = camera;
    ui->previewFirst->setCamera(camera);
}

void SingleCalibrationDialog::takeFrame()
{
    ui->previewFirst->startCameraStream();
    if(camera){
        connect(camera,
                SIGNAL(frameCaptured(cv::Mat,QCamera*,QProjector::Pattern*)),
                this,
                SLOT(frameCaptured(cv::Mat,QCamera*,QProjector::Pattern*)));
        camera->requestFrame();
    }
}

void SingleCalibrationDialog::calibrate()
{
    std::vector<std::vector<cv::Point3f> > objectPoints;

    objectPoints = standard->getObjectPoints(imagePoints.size());

    cv::Mat M = cv::Mat::eye(3,3,CV_64F);
    cv::Mat k = cv::Mat::zeros(1,5,CV_64F);
    std::vector<cv::Mat> rvecs;
    std::vector<cv::Mat> tvecs;

    for(int u=0;u<objectPoints.at(0).size();u++){
        std::cout << objectPoints.at(0).at(u) << '\t'
                  << imagePoints.at(0).at(u) << '\n';
    }

    cv::calibrateCamera(objectPoints,
                        imagePoints,
                        camera->getResolution(),
                        M, k, rvecs, tvecs,
                        CV_CALIB_FIX_ASPECT_RATIO
                      | CV_CALIB_FIX_PRINCIPAL_POINT);

    std::cout << M << '\n' << k << '\n';

    camera->setIntrinsics(M);
    camera->setDistortion(k);

    close();

}


void SingleCalibrationDialog::frameCaptured(cv::Mat frame, QCamera *cam, QProjector::Pattern *)
{
    std::vector<cv::Point2f> pts;
    bool success;
    if(cam!=camera) return;
    camera->disconnect(this);
    pts = standard->getImagePoints(frame,&success);
    if(success){
        imagePoints.push_back(pts);
        ui->lcdNumber->display((int)imagePoints.size());
    }
}

void SingleCalibrationDialog::open()
{
    if(camera) ui->previewFirst->startCameraStream();
    QDialog::open();
}

void SingleCalibrationDialog::close()
{
    ui->previewFirst->stopCameraStream();
    QDialog::close();
}
