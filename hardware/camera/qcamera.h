#ifndef QCAMERA_H
#define QCAMERA_H

#include <QTimer>

#include "../qopticaldevice.h"
#include "../../geom/uniqueimage.h"

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
    virtual bool isOpen(){}

    /// Release any resources this camera is using.
    virtual void release(){}

    /// Set the frame rate.
    void setFrameRate(int fps){}

    /// Get the camera's id
    uint getID(){return id;}

    /// Set the camera's id
    void setID(uint i){id=i;}
    
signals:
    /// A frame has been captured.
    void frameCaptured(cv::Mat frame);

public slots:
    /// Request a frame from the camera.
    virtual void requestFrame(ImageDescriptor desc){}
    /// Start streaming.
    void startStream();
    /// Capture a frame and emit it when it's ready.
    void captureRawFrame();

protected:

private:

    /// Camera ID
    uint id;

    /// Timer for controlling stream
    QTimer *timer;

    /// Is the camera currently streaming?
    bool streaming;
    
};

#endif // QCAMERA_H
