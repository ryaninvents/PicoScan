#include "qfilecamera.h"

#include <opencv2/highgui/highgui.hpp>

QFileCamera::QFileCamera(QString fnm, QObject *parent) :
    QCamera(parent),
    fnm(fnm)
{
}

void QFileCamera::requestFrame(ImageDescriptor desc)
{
    cv::Mat frame;
    if(desc.isColor())
        frame = cv::imread(getFilename(desc));
    else
        frame = cv::imread(getFilename(desc), CV_LOAD_IMAGE_GRAYSCALE);
    processFrame(frame,desc);
}

char *QFileCamera::getFilename(ImageDescriptor desc)
{
    return fnm.arg(desc.getFrameID()).toLocal8Bit().data();
}
