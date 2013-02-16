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

void QCamera::processFrame(cv::Mat frame, ImageDescriptor desc)
{
    UniqueImage im(desc,now(),frame);
    emit frameCaptured(im);
}
