#include "qcamera.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <QDateTime>

QCamera::QCamera(QObject *parent) :
    QOpticalDevice(parent),
    streaming(false)
{
}

void QCamera::startStream()
{
    if(streaming) return;
    timer = new QTimer(this);

}
