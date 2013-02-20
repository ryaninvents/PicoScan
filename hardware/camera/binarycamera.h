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
    /// Create a binary camera. This can't be used without
    /// later passing a capture camera and projector into
    /// setCaptureCamera() and setProjector(), respectively.
    /// \param bit The binary bit to capture.
    /// \param parent An optional QObject to act as the parent.
    explicit BinaryCamera(
            uint bit,
            QObject *parent = 0);
    /// Create the binary camera, ready to go with a
    /// capturing camera and a projector.
    /// \param captureCamera A pointer to a QCamera with which
    ///        to capture raw images.
    /// \param projector A pointer to a projector with which
    ///        to project the necessary patterns.
    /// \param bit The binary bit to capture.
    /// \param parent An optional QObject to act as the parent.
    explicit BinaryCamera(
            QCamera *captureCamera,
            QProjector *projector,
            uint bit,
            QObject *parent = 0);


    /// Is the camera open?
    bool isOpen();

    /// Release the child camera
    void release();

    /// Describe the camera
    QString describe();

    /// Set capture camera
    void setCaptureCamera(QCamera *capCam);

    /// Set projector
    void setProjector(QProjector *pj);

    /// Get the bit this camera is responsible for
    uint getBit();

    
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
    /// The projector is about to advance.
    void projectorAboutToAdvance();
    /// Child camera has taken a picture.
    void rawFrameCaptured(cv::Mat frame,
                          QCamera::FrameType type,
                          QCamera *cam);

private:

    /// Projector
    QProjector *projector;

    /// Capturing camera
    QCamera *camera;

    /// Bit
    uint bit;

    /// Is the camera working on an image?
    bool isWorking;

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

    /// Disconnect existing capture camera
    void disconnectCaptureCamera();

    /// Disconnect existing projector
    void disconnectProjector();

    /// Set up connections in camera
    void connectCaptureCamera();

    /// Set up connections in projector
    void connectProjector();
    
};

#endif // BINARYCAMERA_H
