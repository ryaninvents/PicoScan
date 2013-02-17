#ifndef MONOTRIANGULATOR_H
#define MONOTRIANGULATOR_H

#include "triangulator.h"
#include "sheet.h"
#include "../hardware/camera/pixelencodedcamera.h"

/// A Triangulator that can compute 3D meshes
/// from a single pixel-encoded frame.
class MonoTriangulator : public Triangulator
{
    Q_OBJECT
public:
    /// Create the triangulator
    explicit MonoTriangulator(QObject *parent = 0);

    /// Set the camera
    void setCamera(PixelEncodedCamera *cam);

    /// Set the projector
    void setProjector(QProjector *proj);
    
signals:
    /// The triangulator has computed a Sheet.
    void sheetComputed(Sheet);
    
public slots:
    /// Request a sheet from the triangulator.
    void requestSheet();

    /// A camera frame has returned.
    void frameReturned(cv::Mat frame, QCamera::FrameType);

private:

    /// The camera
    PixelEncodedCamera *camera;
    /// The projector
    QProjector *projector;
    
};

#endif // MONOTRIANGULATOR_H
