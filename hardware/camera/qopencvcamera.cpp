#include "qopencvcamera.h"
#include <opencv2/imgproc/imgproc.hpp>

QOpenCVCamera::QOpenCVCamera(uint idx, QObject *parent) :
    QCamera(parent),
    chuck(2)
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

bool QOpenCVCamera::fetchFrame(QProjector::Pattern *p)
{
    if(!isOpen()) return false;
    getAndEmitFrame(p);
//    QTimer::singleShot(500,this,
//                       SLOT(getAndEmitFrame()));
    return true;
}

void QOpenCVCamera::getAndEmitFrame(QProjector::Pattern *pattern)
{
    uint i;
    cv::Mat m;
    for(i=0;i<chuck;i++){
        cap->read(m);
    }
    cap->read(m);
    emit frameCaptured(m,this,pattern);
    ready();
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
