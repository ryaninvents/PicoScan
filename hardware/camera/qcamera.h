#ifndef QCAMERA_H
#define QCAMERA_H

#include <QTimer>

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
    virtual bool isOpen()=0;

    /// Release any resources this camera is using.
    virtual void release()=0;

    /// Set the frame rate.
    void setFrameRate(uint fps);

    /// Get the camera's id
    uint getID();

    /// Set the camera's id
    void setID(uint i);

    /// Get a detailed description of the camera.
    virtual QString describe()=0;

    /// Different types of frame that you can
    /// request from a camera.
    enum FrameType{
        /// A full-color, 3-channel frame.
        FULL_COLOR,
        /// A grayscale, 64-bit floating-point frame.
        DOUBLE,
        /// A signed, 32-bit integer frame.
        SIGNED_32,
        /// An unsigned, 32-bit integer frame.
        UNSIGNED_16
    };
    
signals:
    /// A frame has been captured.
    void frameCaptured(cv::Mat frame,
                       QCamera::FrameType type,
                       QCamera *cam);

public slots:
    /// Start streaming.
    bool startStream();
    /// Capture a frame and emit it when it's ready.
    /// \returns \b true if the camera will emit the
    /// frame, \b false if the camera is incapable of
    /// emitting the desired type of frame.
    virtual bool requestFrame(QCamera::FrameType)=0;
    /// Capture a full-color frame and emit when ready.
    bool requestFrame();

protected:

    /// Convert an image type to an OpenCV flag.
    static int getOpenCVFlagFromType(QCamera::FrameType type);

private:

    /// Camera ID
    uint id;

    /// Timer for controlling stream
    QTimer *timer;

    /// Is the camera currently streaming?
    bool streaming;

    /// Frame interval
    uint interval;
    
};

#endif // QCAMERA_H
