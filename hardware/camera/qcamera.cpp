#include "qcamera.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <QDateTime>

QCamera::QCamera(QObject *parent) :
    QOpticalDevice(parent),
    streaming(false),
    interval(1000/30),
    readyToAdvance(true)
{
}

void QCamera::setFrameRate(uint fps)
{
    interval = 1000/fps;
    if(streaming){
        timer->setInterval(interval);
    }
}

uint QCamera::getID()
{
    return id;
}

void QCamera::setID(uint i)
{
    id=i;
}

void QCamera::setProjector(QProjector *p)
{
    projector = p;
    connect(projector,
            SIGNAL(patternProjected(QProjector::Pattern*,QProjector*)),
            this,
            SLOT(requestFrame(QProjector::Pattern*,QProjector*)));
    connect(projector,
            SIGNAL(aboutToAdvance()),
            this,
            SLOT(projectorAboutToAdvance()));
    connect(this,
            SIGNAL(projectorPermissionToAdvance(bool)),
            projector,
            SLOT(permissionToAdvance(bool)));
}

QProjector *QCamera::getProjector()
{
    return projector;
}

bool QCamera::startStream()
{
    return false;
    if(streaming) return false;
    timer = new QTimer(this);
    connect(timer,
            SIGNAL(timeout()),
            this,
            SLOT(requestFrame()));
    timer->start();
    return true;
}

bool QCamera::requestFrame(QProjector::Pattern *pat, QProjector *proj)
{
    //if(proj!=projector) return false;
    readyToAdvance = false;
    pattern = pat;
    return fetchFrame(pat);
}

bool QCamera::requestFrame()
{
    return fetchFrame(0);
}

void QCamera::projectorAboutToAdvance()
{
    emit projectorPermissionToAdvance(readyToAdvance);
}

void QCamera::emitFrame(cv::Mat frame)
{
    emit frameCaptured(frame,this,pattern);
}

void QCamera::ready()
{
    readyToAdvance = true;
    projector->requestAdvance();
}
