#ifndef QOPENCVCAMERA_H
#define QOPENCVCAMERA_H

#include "qcamera.h"
#include <opencv2/highgui/highgui.hpp>

class QOpenCVCamera : public QCamera
{
    Q_OBJECT
public:
    /// Create a new camera with the given OpenCV index.
    explicit QOpenCVCamera(uint idx,QObject *parent = 0);
    
    /// Is this camera available to take frames?
    bool isOpen();

    /// Release the camera.
    void release();

    /// Set frame rate.
    /// \bug This method does not seem to actually do anything.
    void setFrameRate(uint fps);

signals:
    
public slots:
    /// Request a frame from the camera.
    bool requestFrame(FrameType type);

private:

    /// The video capture device.
    cv::VideoCapture *cap;
    
};

#endif // QOPENCVCAMERA_H
