#include "qcamera.h"

#include <opencv2/imgproc/imgproc.hpp>

QCamera::QCamera(QObject *parent) :
    QOpticalDevice(parent)
{
}
