#include "qcamera.h"

#include <opencv2/imgproc/imgproc.hpp>

QCamera::QCamera(QObject *parent) :
    QOpticalDevice(parent)
{
}



cv::Mat QCamera::getFrameBW()
{
    cv::Mat frame = getFrame();
    cv::cvtColor(frame,frame,CV_RGB2GRAY);
    frame.assignTo(frame,CV_32S);
    return frame;
}

cv::Mat QCamera::getFrameBW(unsigned int n)
{
    cv::Mat out = cv::Mat::zeros(getResolutionU(),
                                         getResolutionV(),
                                         CV_32S);
//    return out;
    //*
    unsigned int i;
    for(i=0;i<n;i++){
//        out = out +
                getFrameBW();
    }
    //out = out/n;//*/
//    out.convertTo(out,CV_32S);
//    return out;
    return getFrameBW();
}
