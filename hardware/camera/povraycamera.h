#ifndef POVRAYCAMERA_H
#define POVRAYCAMERA_H

#include "qcamera.h"

/// A POV-Ray camera used for simulation.
///
/// <a href="http://www.povray.org/">POV-Ray</a>
/// is a powerful procedural ray-tracing program.
/// By writing scripts, you can simulate the
/// effects of light interaction very precisely.
///
/// POV-Ray maintains <a href="http://bit.ly/VZep0l">
/// a list of model repositories</a> you can use
/// to test out your scanning methods. Ultimately,
/// though, you're going to want to simulate a perfect
/// plane or sphere--something you can mathematically
/// compute deviations with so you can validate your
/// method before moving to real hardware.
///
/// Technically you can stream from this camera,
/// but you're likely going to wait quite
/// a while between frames depending on your
/// setup.
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

    /// Set simulated X
    void setSimX(double x);

    /// Set simulated Y
    void setSimY(double y);

    /// Set simulated Z
    void setSimZ(double z);

    /// Set simulated orientation (Rodrigues vector)
    void setSimOrientation(cv::Vec3d r);

    /// Set simulated focal length
    void setSimFocalLength(double f);

    /// Set simulated cell size
    void setSimCellSize(double c);

    /// Set antialiasing value
    void setAntialiasing(int aa);

    /// Set parameter filename
    void setParameterFilename(QString fnm);

    /// Set ini filename
    void setIniFilename(QString fnm);

    /// Set scene filename
    void setSceneFilename(QString fnm);

    /// Set render filename
    void setRenderFilename(QString fnm);

    /// Describe the camera in detail
    QString describe();

signals:
    /// A frame has been captured.
    void frameCaptured(cv::Mat frame,QCamera::FrameType type);

public slots:
    /// Write camera parameters to disk, render
    /// the scene, and emit the frame when it's
    /// ready.
    bool requestFrame(QCamera::FrameType type);

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

    /// Simulated focal length, real units
    double simFocalLength;
    
};

#endif // POVRAYCAMERA_H
