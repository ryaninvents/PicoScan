#ifndef BINARYCAPTURECAMERA_H
#define BINARYCAPTURECAMERA_H

#include "qcamera.h"
#include "../projector/qprojector.h"

/// A "binary camera", which is capable of capturing
/// a binary frame all by itself.
/// Just pass in a QCamera* and a QProjector* to use
/// for capture and projection, respectively. If
/// there is another BinaryCaptureCamera working,
/// then this one will hook into the same projector
/// pattern queue.
/// One thing to note: while it is possible to stream
/// from a BinaryCaptureCamera, you're not really
/// going to have a whole lot of fun doing it. You'll
/// probably max out around 1 frame every 10 seconds
/// or so. Don't say I didn't warn you...

class BinaryCaptureCamera : public QCamera
{
    Q_OBJECT
public:

    /// Create a new BinaryCaptureCamera.
    explicit BinaryCaptureCamera(QObject *parent = 0);

    /// Set high and low bits simultaneously.
    /// \returns \b true if the bits were set,
    /// \b false if not. A failure might indicate
    /// that \b loBit &ge; \b hiBit.
    bool setBitRange(uint lo, uint hi);
    
signals:
    
public slots:

    /// Capture a frame and emit when ready.
    /// BinaryCaptureCamera will only emit grayscale
    /// binary-encoded frames.
    /// \returns \b true if the camera will emit the
    /// frame, \b false if the camera will not. Likely
    /// reasons for a false return value: color frame
    /// request, or capture already in progress.
    bool requestFrame(FrameType type);

    /// A particular pattern has been projected.
    void patternProjected(QProjector::Pattern *pattern);

private:

    /// the camera we will capture with
    QCamera *camera;

    /// the projector we will be using for binary patterns
    QProjector *projector;

    /// the lowest bit we will capture
    uint loBit;

    /// the highest bit we will capture
    uint hiBit;

    /// the frame index we're waiting to fill
    uint currentFrameIndex;

    /// the frames we capture, before processing
    std::vector<cv::Mat> frames;

    /// the type of frame we've been tasked with getting
    FrameType type;

    /// is there a capture in progress?
    bool inProgress;

    
};

#endif // BINARYCAPTURECAMERA_H
