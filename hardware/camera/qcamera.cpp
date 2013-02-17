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
