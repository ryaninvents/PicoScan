#ifndef PIXELENCODEDCAMERA_H
#define PIXELENCODEDCAMERA_H

#include "qcamera.h"
#include "../projector/qprojector.h"

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
signals:
    
public slots:

private:

    /// the camera we will capture with
    QCamera *camera;

    /// the projector we will be using for binary patterns
    QProjector *projector;
    
};

#endif // PIXELENCODEDCAMERA_H
