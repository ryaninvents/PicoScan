#ifndef BINARYCAMERA_H
#define BINARYCAMERA_H

#include "qcamera.h"
#include "../projector/qprojector.h"

/// Represents a camera that captures a binary frame
/// on command.
class BinaryCamera : public QCamera
{
    Q_OBJECT
public:
    /// Create the binary camera.
    explicit BinaryCamera(QObject *parent = 0);

    /// Is the camera open?
    bool isOpen();


    
signals:
    
public slots:
    /// Request a frame.
    bool requestFrame(QCamera::FrameType type);

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
    
};

#endif // BINARYCAMERA_H
