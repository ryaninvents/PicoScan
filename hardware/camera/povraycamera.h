#ifndef POVRAYCAMERA_H
#define POVRAYCAMERA_H

#include "qcamera.h"

/// A POV-Ray camera used for simulation.
/// Technically you can stream from this,
/// but you're likely going to wait quite
/// a while between frames.
class PovRayCamera : public QCamera
{
    Q_OBJECT
public:

    /// Create the camera.
    explicit PovRayCamera(QObject *parent = 0);

    /// The camera is always open
    bool isOpen();

    /// Release resources; that is, do nothing.
    /// It's fine that the files linger in the
    /// system; they can yield useful data.
    void release();

signals:
    
public slots:
    /// Write camera parameters to disk, render
    /// the scene, and emit the frame when it's
    /// ready.
    bool requestFrame(FrameType type);

private:

    /// The filename to write camera parameters to
    QString paramFilename;

    /// The filename of the scene file
    QString sceneFilename;

    /// The filename with the INI parameters
    QString iniFilename;

    /// The filename to render to/read from
    QString renderFilename;

    /// Simulated position
    cv::Vec3d simPosition;

    /// Simulated orientation, in Rodrigues notation.
    /// Basically, the vector represents an axis, and
    /// its length represents the amount to rotate.
    cv::Vec3d simRotation;

    /// Simulated cell size; you probably don't have
    /// to worry about this. Even if it's different
    /// from the actual value the simulation will be
    /// identical.
    double simCellSize;

    /// Anti-aliasing
    int antialiasing;
    
};

#endif // POVRAYCAMERA_H
