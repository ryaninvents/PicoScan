#include "camera.h"

#include <opencv2/imgproc/imgproc.hpp>

Camera::Camera()
{
    setName("Camera");
}

cv::Mat Camera::getFrameBW()
{
    cv::Mat frame = getFrame();
    cv::cvtColor(frame,frame,CV_RGB2GRAY);
    frame.assignTo(frame,CV_8U);
}
