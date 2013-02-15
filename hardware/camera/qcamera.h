#ifndef QCAMERA_H
#define QCAMERA_H

#include "../qopticaldevice.h"

/**
  Represents a generic camera. The specifics
  of obtaining an image are left up to the
  implementation of child classes.
  */

class QCamera : public QOpticalDevice
{
    Q_OBJECT
public:
    /// Instantiate a camera.
    explicit QCamera(QObject *parent = 0);

    /// Is the camera available to take frames?
    virtual bool isOpen() = 0;

    /// Release any resources this camera is using.
    virtual void release() = 0;

    /// Set the frame rate.
    void setFrameRate(int fps){}

    /// Get the camera's id
    uint getID(){return id;}

    /// Set the camera's id
    void setID(uint i){id=i;}
    
signals:
    /// Signal that a frame has been captured.
    void frameCaptured(cv::Mat frame,
                       qint64 timestamp,
                       uint frameID,
                       uint cameraID);
    /// Signal that a grayscale frame has been captured.
    void frameCapturedGray(cv::Mat frame,
                           qint64 timestamp,
                           uint frameID,
                           uint cameraID);

public slots:
    /// Request a frame from the camera.
    virtual void requestFrame(uint frameID);

    /// Request a grayscale frame.
    virtual void requestGrayscaleFrame(uint frameID);

protected:

    /// Get the current time in milliseconds.
    qint64 now();


private:
    /// Current mode
    CameraMode mode;

    /// Camera ID
    uint id;
    
};

#endif // QCAMERA_H
