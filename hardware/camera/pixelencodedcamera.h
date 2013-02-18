#ifndef PIXELENCODEDCAMERA_H
#define PIXELENCODEDCAMERA_H

#include "qcamera.h"
#include "../projector/qprojector.h"
#include "../projector/projectordependent.h"

/// A camera that outputs frames encoded with
/// pixel information from a projector. Exactly
/// how the encoding is performed is left up to
/// subclasses, but the idea is that the encoded
/// value will represent the projector-u of the
/// ray that illuminated that pixel. This will
/// ensure the output plays nicely with
/// Triangulator.
class PixelEncodedCamera : public QCamera
{
    Q_OBJECT
public:
    /// Create a new pixel-encoded camera.
    explicit PixelEncodedCamera(QObject *parent = 0);

    /// Set the camera with which to capture the frames.
    void setCapturingCamera(QCamera *cam);

    /// Get the capturing camera.
    QCamera *getCapturingCamera();

    /// Set the projector with which to project the binary patterns.
    void setProjector(QProjector *proj);

    /// Get the projector
    QProjector *getProjector();

    /// Is there a capture in progress?
    bool isScanInProgress();

    /// Requested frame type
    FrameType getFrameType();

signals:
    
public slots:
    /// A frame has been returned.
    void frameReturned(cv::Mat frame, FrameType);

protected:

    /// Provide space for frames
    void allocateFrames(uint n);

    /// Get a particular captured frame
    cv::Mat getCapturedFrame(uint idx);

    /// Set a particular captured frame
    void setCapturedFrame(uint idx,cv::Mat frame);

    /// Clear captured frames
    void clearCapturedFrames();

    /// Have we captured the \a ith frame?
    bool hasCapturedFrame(uint i);

    /// Have we captured all the frames we need?
    bool capturedAllFrames();

    /// How many frames have we allocated?
    uint framesAllocated();

    /// Let the PixelEncodedCamera know we're waiting for
    /// a particular frame.
    void waitingFor(uint frame);

    /// Compile the frames into an encoded frame.
    virtual void compileFrames() = 0;

private:

    /// the camera we will capture with
    QCamera *camera;

    /// the projector we will be using for binary patterns
    QProjector *projector;

    /// show dependency between the camera and projector
    /// so that the projector waits for the camera before
    /// moving on to the next frame
    ProjectorDependent *dpcy;

    /// The frames in sequence
    std::vector<cv::Mat> frames;

    /// Whether each frame has been captured or not
    std::vector<bool> hasBeenCap;

    /// whether we have a scan in progress
    bool inProgress;

    /// the type of frame we're working on
    FrameType requestedType;

    /// the frame we're waiting for
    uint frameIdx;
    
};

#endif // PIXELENCODEDCAMERA_H
