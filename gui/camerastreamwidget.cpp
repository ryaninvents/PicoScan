#include "camerastreamwidget.h"

CameraStreamWidget::CameraStreamWidget(QWidget *parent) :
    ImageViewWidget(parent),
    running(false),
    interval(250),
    camera(0)
{
}

void CameraStreamWidget::setCamera(QCamera *cam)
{
    if(camera) camera->disconnect(this);
    camera = cam;
    connect(camera,
            SIGNAL(frameCaptured(cv::Mat,QCamera*,QProjector::Pattern*)),
            this,
            SLOT(frameCaptured(cv::Mat,QCamera*,QProjector::Pattern*)));
}

void CameraStreamWidget::startCameraStream()
{
    if(!camera || running) return;
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(updateImage()));
    timer->start(interval);
    running = true;
}

void CameraStreamWidget::stopCameraStream()
{
    if(!running || !camera) return;
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
    if(!camera || !running) return;
    camera->requestFrame();
}

void CameraStreamWidget::closeCameraStream()
{
    stopCameraStream();
}

void CameraStreamWidget::frameCaptured(cv::Mat frame, QCamera *cam, QProjector::Pattern *)
{
    if(cam!=camera || !running) return;
    displayMat(frame,true);
}
