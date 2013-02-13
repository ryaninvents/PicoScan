#ifndef OPENCVCAMERA_H
#define OPENCVCAMERA_H

#include "camera.h"
#include <opencv2/highgui/highgui.hpp>

/**
  Represents a camera that uses OpenCV's
  HighGUI interface.
  */
class OpenCVCamera: public Camera
{
public:
    /**
      Create a camera by selecting its index.
      */
    OpenCVCamera(int idx);
    /**
      Grab a frame from the camera.
    */
    cv::Mat getFrame();
    /**
      Determine whether the camera is available to take frames.
      */
    bool isOpen(){return cap->isOpened();}

    void release();

    /** Set the capture resolution of this device. */
    void setResolution(int u,int v);

    /** Set the capture resolution of this device. */
    void setResolution(cv::Size s){setResolution(s.width,s.height);}

    /** Returns the horizontal resolution of this camera. */
    int getResolutionU();

    /** Returns the vertical resolution of this camera. */
    int getResolutionV();

    /** Returns the resolution of this camera.    */
    cv::Size getResolution(){Camera::setResolution(getResolutionU(),
                                                   getResolutionV());
                            return Camera::getResolution();}

    /// Set frame rate.
    void setFrameRate(int fps);

private:
    cv::VideoCapture *cap;
};

#endif // OPENCVCAMERA_H
