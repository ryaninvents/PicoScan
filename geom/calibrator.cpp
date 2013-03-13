#include "calibrator.h"

Calibrator::Calibrator(QObject *parent) :
    QObject(parent)
{
}

void Calibrator::setLeft(QCamera *cam)
{
    leftCam = cam;
}

void Calibrator::setRight(QCamera *cam)
{
    rightCam = cam;
}

void Calibrator::frameCaptured(cv::Mat frame, QCamera *cam, QProjector::Pattern *)
{
    if(cam==leftCam){
        framesLeft.push_back(frame);
        leftCam->disconnect(this);
    }else if(cam==rightCam){
        framesRight.push_back(frame);
        rightCam->disconnect(this);
    }
}

void Calibrator::takeStereoFrame()
{
    if(!leftCam || !rightCam) return;
    connect(leftCam,
            SIGNAL(frameCaptured(cv::Mat,QCamera*,QProjector::Pattern*)),
            this,
            SLOT(frameCaptured(cv::Mat,QCamera*,QProjector::Pattern*)));
    connect(rightCam,
            SIGNAL(frameCaptured(cv::Mat,QCamera*,QProjector::Pattern*)),
            this,
            SLOT(frameCaptured(cv::Mat,QCamera*,QProjector::Pattern*)));
    leftCam->requestFrame();
    rightCam->requestFrame();

}
