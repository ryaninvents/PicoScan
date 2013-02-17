#include "qcamera.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <QDateTime>

QCamera::QCamera(QObject *parent) :
    QOpticalDevice(parent),
    streaming(false),
    interval(1000/30)
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

bool QCamera::startStream()
{
    if(streaming) return false;
    timer = new QTimer(this);
    connect(timer,
            SIGNAL(timeout()),
            this,
            SLOT(requestFrame()));
    return true;
}

int QCamera::getOpenCVFlagFromType(QCamera::FrameType type)
{
    switch(type){
    case FULL_COLOR:
        return CV_8UC3;
    case DOUBLE:
        return CV_64F;
    case SIGNED_32:
        return CV_32S;
    case UNSIGNED_16:
        return CV_16U;
    }
    return 0;
}
