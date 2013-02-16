#ifndef QCAMERA_H
#define QCAMERA_H

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

    /// Signal that a frame has been captured.
    /// Subclasses shouldn't emit this signal directly;
    /// instead, use processFrame().
    void frameCaptured(UniqueImage im);

public slots:
    /// Request a frame from the camera.
    virtual void requestFrame(ImageDescriptor desc){}

protected:

    /// Get the current time in milliseconds.
    qint64 now();

    /// Process a frame and emit it.
    void processFrame(cv::Mat frame, ImageDescriptor desc);


private:

    /// Camera ID
    uint id;
    
};

#endif // QCAMERA_H
