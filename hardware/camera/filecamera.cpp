#include "filecamera.h"

#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>

FileCamera::FileCamera(char *finm):
    Camera(),
    fnm(finm),
    idx(0)
{
}

void FileCamera::setMode(CameraMode m)
{
    Camera::setMode(m);
    idx = 0;
}

cv::Mat FileCamera::getFrame()
{
    char *fnm = getNextFilename();
    printf("%s\n",fnm);
    cv::Mat frame = cv::imread(fnm);
    //frame.convertTo(frame,CV_16U);
    return frame;
}

cv::Mat FileCamera::getFrameBW()
{
    char *fnm = getNextFilename();
    printf("%s\n",fnm);
    cv::Mat frame = cv::imread(fnm,CV_LOAD_IMAGE_GRAYSCALE);
    frame.convertTo(frame,CV_16U);
    return frame;
}



bool FileCamera::isOpen()
{
    return true;
}

char *FileCamera::getEnumValueAsString()
{
    switch(getMode()){
    case CAMERA_CALIBRATION: return "camcal";
    case PROJECTOR_CALIBRATION: return "procal";
    case BINARY_CAPTURE: return "bincap";
    case FRINGE_CAPTURE: return "fgecap";
    }
    return 0;
}

char *FileCamera::getNextFilename()
{
    char buffer[256];
    sprintf(buffer,"%s%d.png","/home/ryan/Downloads/2013 Feb 13/Round 1/*-",idx);
    printf("filename\t\t%s\n",buffer);
    idx++;
    if(idx>22) idx=0;
    return buffer;
}
