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



void OpenCVCamera::setResolution(int u, int v)
{
    cap.set(CV_CAP_PROP_FRAME_WIDTH,u);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,v);
    Camera::setResolution(u,v);
}

int OpenCVCamera::getResolutionU()
{
    return cap.get(CV_CAP_PROP_FRAME_WIDTH);
}

int OpenCVCamera::getResolutionV()
{
    return cap.get(CV_CAP_PROP_FRAME_HEIGHT);
}
