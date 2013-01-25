#include "opencvcamera.h"

OpenCVCamera::OpenCVCamera(int idx):
    cap(idx)
{
    setResolution(cap.get(CV_CAP_PROP_FRAME_WIDTH),
                  cap.get(CV_CAP_PROP_FRAME_HEIGHT));
}

cv::Mat OpenCVCamera::getFrame()
{
    cv::Mat out;
    cap >> out;
    return out;
}
