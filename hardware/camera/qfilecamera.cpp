#include "qfilecamera.h"

#include <opencv2/highgui/highgui.hpp>

QFileCamera::QFileCamera(QString fnm, QObject *parent) :
    QCamera(parent),
    fnm(fnm)
{
}

void QFileCamera::requestFrame(uint frameID)
{
    cv::Mat m = cv::imread(fnm.arg(frameID).toLocal8Bit().data());
    frameCaptured(m,now(),frameID,getID());
}

void QFileCamera::requestGrayscaleFrame(uint frameID)
{
    cv::Mat m = cv::imread(fnm.arg(frameID).toLocal8Bit().data(),
                           CV_LOAD_IMAGE_GRAYSCALE);
    frameCaptured(m,now(),frameID,getID());
}
