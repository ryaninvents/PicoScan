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

void QOpenCVCamera::requestFrame(uint frameID)
{
    cv::Mat m;
    cap->read(m);
    frameCaptured(m,now(),frameID,getID());
}

void QOpenCVCamera::requestGrayscaleFrame(uint frameID)
{
    cv::Mat m;
    cap->read(m);
    cv::cvtColor(m,m,CV_RGB2GRAY);
    frameCapturedGray(m,now(),frameID,getID());
}
