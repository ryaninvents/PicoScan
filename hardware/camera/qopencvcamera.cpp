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

QString QOpenCVCamera::describe()
{
    return QString("OpenCV Camera");
}

bool QOpenCVCamera::requestFrame(FrameType type)
{
    if(!isOpen()) return false;
    cv::Mat m;
    cap->read(m);
    switch(type){
    case FULL_COLOR:
        break;
    case DOUBLE:
        cv::cvtColor(m,m,CV_RGB2GRAY);
        m.convertTo(m,CV_64F);
        break;
    case SIGNED_32:
        cv::cvtColor(m,m,CV_RGB2GRAY);
        m.convertTo(m,CV_32S);
        break;
    case UNSIGNED_16:
        cv::cvtColor(m,m,CV_RGB2GRAY);
        m.convertTo(m,CV_16U);
        break;
    }
    emit frameCaptured(m,type,this);
    return true;
}

void QOpenCVCamera::adjustDeviceResolution(int u, int v)
{
    bool uSuccess, vSuccess;
    uSuccess = cap->set(CV_CAP_PROP_FRAME_WIDTH,u);
    vSuccess = cap->set(CV_CAP_PROP_FRAME_HEIGHT,v);
    emit debug(QString("Horiz res change: %1\n"
                       "Vert res change: %2")
               .arg(uSuccess?"success":"failure")
               .arg(vSuccess?"success":"failure"));
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
