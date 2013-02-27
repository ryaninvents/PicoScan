#include "qopencvcamera.h"
#include <opencv2/imgproc/imgproc.hpp>

QOpenCVCamera::QOpenCVCamera(uint idx, QObject *parent) :
    QCamera(parent)
{
    cap = new cv::VideoCapture();
    cap->open(idx);
    connect(this,
            SIGNAL(resolutionChanged(int,int)),
            this,
            SLOT(adjustDeviceResolution(int,int)));
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

void QOpenCVCamera::setFrameRate(uint fps)
{
    cap->set(CV_CAP_PROP_FPS,fps);
    QCamera::setFrameRate(fps);
}

bool QOpenCVCamera::fetchFrame()
{
    if(!isOpen()) return false;
    getAndEmitFrame();
//    QTimer::singleShot(500,this,
//                       SLOT(getAndEmitFrame()));
    return true;
}

void QOpenCVCamera::getAndEmitFrame()
{
    cv::Mat m;
    cap->read(m);
    emitFrame(m);
}

void QOpenCVCamera::adjustDeviceResolution(int u, int v)
{
    bool uSuccess, vSuccess;
    uSuccess = cap->set(CV_CAP_PROP_FRAME_WIDTH,u);
    vSuccess = cap->set(CV_CAP_PROP_FRAME_HEIGHT,v);
    emit debug(QString("Horiz res: %1\n"
                       "Vert res: %2")
               .arg(getResolutionU())
               .arg(getResolutionV()));
}

int QOpenCVCamera::getResolutionU()
{
    return cap->get(CV_CAP_PROP_FRAME_WIDTH);
}

int QOpenCVCamera::getResolutionV()
{
    return cap->get(CV_CAP_PROP_FRAME_HEIGHT);
}

cv::Size QOpenCVCamera::getResolution()
{
    return cv::Size(getResolutionU(),
                    getResolutionV());
}
