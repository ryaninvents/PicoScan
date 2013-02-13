#include "filecamera.h"

#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>

#define FILE_PATTERN "%s%s%4d.png"

FileCamera::FileCamera(char *fnm):
    Camera(),
    fnm(fnm),
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
    return cv::imread(getNextFilename());
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
    char buffer[128];
    sprintf(buffer,FILE_PATTERN,getEnumValueAsString(),idx);
    idx++;
    return buffer;
}
