#ifndef BINARYCAMERA_H
#define BINARYCAMERA_H

#include "qcamera.h"
#include "../projector/qprojector.h"
#include "../projector/graycodepattern.h"

/// Represents a camera that captures a binary frame
/// on command, projecting two GrayCodePatterns and
/// finding the difference.
class BinaryCamera : public QCamera
{
    Q_OBJECT
public:
    /// Create the binary camera.
    explicit BinaryCamera(uint bit,
                          QObject *parent = 0);

    /// Is the camera open?
    bool isOpen();

    /// Describe the camera
    QString describe();


    
signals:
    /// Allow or deny the projector permission to
    /// advance.
    void allowProjectorToAdvance(bool);
    
public slots:
    /// Request a frame.
    bool requestFrame(QCamera::FrameType type);
    /// A pattern has been projected.
    void patternProjected(QProjector::Pattern *p,
                          QProjector *pj);
    /// A frame has been captured from the
    /// capturing camera.
    void frameCaptured(cv::Mat frame,
                       QCamera::FrameType type,
                       QCamera *cam);
    /// The projector is about to advance.
    void projectorAboutToAdvance();

private:

    /// Projector
    QProjector *projector;

    /// Capturing camera
    QCamera *camera;

    /// Bit
    uint bit;

    /// Is the camera working on an image?
    bool isWorking;

    /// Threshold
    uint threshold;

    /// Current projected pattern
    GrayCodePattern *pattern;

    /// Positive frame
    cv::Mat positive;

    /// Negative frame
    cv::Mat negative;

    /// Have we captured the positive frame?
    bool havePositive;

    /// Have we captured the negative frame?
    bool haveNegative;

    /// Do we want the projector to advance?
    bool projectorCanAdvance;
    
};

#endif // BINARYCAMERA_H
