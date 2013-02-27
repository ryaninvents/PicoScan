#ifndef QCAMERA_H
#define QCAMERA_H

#include <QTimer>

#include "../qopticaldevice.h"
#include "../projector/qprojector.h"
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

    /// Set the projector this camera is tied to
    void setProjector(QProjector *p);

signals:
    /// A frame has been captured.
    void frameCaptured(cv::Mat frame,
                       QCamera *cam,
                       QProjector::Pattern *pattern);

public slots:
    /// Start streaming.
    bool startStream();
    /// Capture a frame and emit it when it's ready.
    /// Connect this to a projector's patternProjected()
    /// signal to have the projector trigger the camera.
    /// \returns \b true if the camera will emit the
    /// frame, \b false if the camera is incapable of
    /// emitting the desired type of frame.
    bool requestFrame(QProjector::Pattern *pat, QProjector *proj);

protected:
    /// Fetch and emit a frame
    virtual bool fetchFrame()=0;

    /// Emit a frame
    void emitFrame(cv::Mat frame);

private:

    /// Camera ID
    uint id;

    /// Timer for controlling stream
    QTimer *timer;

    /// Is the camera currently streaming?
    bool streaming;

    /// Frame interval
    uint interval;

    /// current pattern
    QProjector::Pattern *pattern;

    /// master projector
    QProjector *projector;
    
};

#endif // QCAMERA_H
