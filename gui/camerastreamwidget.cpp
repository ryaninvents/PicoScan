#include "camerastreamwidget.h"

CameraStreamWidget::CameraStreamWidget(QWidget *parent) :
    ImageViewWidget(parent),
    running(false),
    interval(100)
{
    noCamera = true;
}

void CameraStreamWidget::startCameraStream()
{
    if(noCamera || running) return;
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(updateImage()));
    timer->start(interval);
    running = true;
}

void CameraStreamWidget::stopCameraStream()
{
    if(!running || noCamera) return;
    timer->stop();
    running = false;
}

void CameraStreamWidget::toggleCameraStream(bool b)
{
    if(b){
        startCameraStream();
    }else{
        stopCameraStream();
    }
}

void CameraStreamWidget::updateImage()
{
    if(noCamera) return;
    cv::Mat3b image;
    image = camera->getFrame();
    displayImage(image,true);
}

void CameraStreamWidget::closeCameraStream()
{
    stopCameraStream();
}
