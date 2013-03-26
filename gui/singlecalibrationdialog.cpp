#include "singlecalibrationdialog.h"
#include "ui_singlecalibrationdialog.h"

SingleCalibrationDialog::SingleCalibrationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SingleCalibrationDialog),
    numFrames(0)
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
}


void SingleCalibrationDialog::frameCaptured(cv::Mat frame, QCamera *cam, QProjector::Pattern *pattern)
{
    std::vector<cv::Point2f> pts;
    bool success;
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
