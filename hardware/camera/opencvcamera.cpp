#include "opencvcamera.h"

#include <stdio.h>

OpenCVCamera::OpenCVCamera(int idx)
{
    cap = new cv::VideoCapture(idx);
    setResolution(1024,768);
    setFrameRate(1);
    /*printf("Camera %d: %dx%d\n",
           idx,
           getResolutionU(),
           getResolutionV());*/
}

cv::Mat OpenCVCamera::getFrame()
{
    cv::Mat out;
    cap->read(out);
    return out;
}



void OpenCVCamera::setResolution(int u, int v)
{
    cap->set(CV_CAP_PROP_FRAME_WIDTH,u);
    cap->set(CV_CAP_PROP_FRAME_HEIGHT,v);
    u = cap->get(CV_CAP_PROP_FRAME_WIDTH);
    v = cap->get(CV_CAP_PROP_FRAME_HEIGHT);
    Camera::setResolution(u,v);
}

int OpenCVCamera::getResolutionU()
{
    return cap->get(CV_CAP_PROP_FRAME_WIDTH);
}

int OpenCVCamera::getResolutionV()
{
    return cap->get(CV_CAP_PROP_FRAME_HEIGHT);
}

void OpenCVCamera::setFrameRate(int fps)
{
    cap->set(CV_CAP_PROP_FPS,fps);
}

void OpenCVCamera::release()
{
    cap->release();
    delete cap;
    cap = 0;
}
