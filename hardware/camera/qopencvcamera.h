#ifndef QOPENCVCAMERA_H
#define QOPENCVCAMERA_H

#include "qcamera.h"
#include <opencv2/highgui/highgui.hpp>

/// OpenCV camera using QObject capabilities
/// to (hopefully) stay in sync with the projector.
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

    /// Returns the horizontal resolution of this device.
    int getResolutionU();

    /// Returns the vertical resolution of this device.
    int getResolutionV();

    /// Returns the resolution of this device.
    cv::Size getResolution();

signals:
    
public slots:

    /// Adjust the device's resolution.
    void adjustDeviceResolution(int u, int v);
    void getAndEmitFrame(QProjector::Pattern *pattern);

protected:
    /// get a frame
    bool fetchFrame(QProjector::Pattern* p);

private:

    /// The video capture device.
    cv::VideoCapture *cap;

    /// Number of frames to discard.
    uint chuck;

    
};

#endif // QOPENCVCAMERA_H
