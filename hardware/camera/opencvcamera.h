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
    bool isOpen(){return cap.isOpened();}
private:
    cv::VideoCapture cap;
};

#endif // OPENCVCAMERA_H
