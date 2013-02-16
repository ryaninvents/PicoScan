#include "qopencvcamera.h"
#include <opencv2/imgproc/imgproc.hpp>

QOpenCVCamera::QOpenCVCamera(uint idx, QObject *parent) :
    QCamera(parent),
    cap(new cv::VideoCapture(idx))
{
}

bool QOpenCVCamera::isOpen()
{
    return cap->isOpened();
}

void QOpenCVCamera::release()
{
    cap->release();
    delete cap;
}

void QOpenCVCamera::setFrameRate(int fps)
{
    cap->set(CV_CAP_PROP_FPS,fps);
}

void QOpenCVCamera::requestFrame(ImageDescriptor desc)
{
    cv::Mat m;
    cap->read(m);
    if(!desc.isColor())
        cv::cvtColor(m,m,CV_RGB2GRAY);
    frameCaptured(m,now(),frameID,getID());
}
