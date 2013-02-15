#include "qcamera.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <QDateTime>

QCamera::QCamera(QObject *parent) :
    QOpticalDevice(parent)
{
}

qint64 QCamera::now()
{
    return QDateTime::currentMSecsSinceEpoch();
}
