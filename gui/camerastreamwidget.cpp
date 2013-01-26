#include "camerastreamwidget.h"

CameraStreamWidget::CameraStreamWidget(QWidget *parent) :
    ImageViewWidget(parent),
    running(false),
    interval(30)
{
}

void CameraStreamWidget::startCameraStream()
{
    if(running) return;
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(updateImage()));
    timer->start(interval);
    running = true;
}

void CameraStreamWidget::stopCameraStream()
{
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
    cv::Mat3b image;
    image = camera->getFrame();
    displayImage(image,true);
}
