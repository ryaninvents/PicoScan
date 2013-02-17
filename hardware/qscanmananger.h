#ifndef QSCANMANANGER_H
#define QSCANMANANGER_H

#include <QObject>
#include <opencv2/core/core.hpp>
#include "projector/qprojector.h"
#include "camera/qcamera.h"
#include "standards/calibrationstandard.h"

/// Scan manager using signals and slots.
class QScanMananger : public QObject
{
    Q_OBJECT
public:
    /// Create a new ScanManager
    explicit QScanMananger(QObject *parent = 0);

    /// Set the projector to use.
    void setProjector(QProjector *p);

    /// Get the projector
    QProjector *getProjector();

    /// Set the calibration standard
    void setStandard(CalibrationStandard *std);
    /// Get the current calibration standard
    CalibrationStandard *getStandard();

    /// Get a specific camera
    QCamera *getCamera(uint i);

    /// Add a camera to the manager
    void setCamera(uint i, QCamera *camera);

    /// Count the cameras
    uint numCameras();

    /// If there are more than \a n cameras, remove some until there's
    /// at most \a n.
    void limitNumCameras(int n);

    /// Get the first camera.
    /// \deprecated use getCamera(0)
    QCamera *getFirst();

    /// Get the second camera.
    /// \deprecated use getCamera(1)
    QCamera *getSecond();

    /// Is this manager stereo?
    /// \deprecated use numCameras()>1
    bool isStereo();
    
signals:
    /// Request a grayscale frame. Will be passed back in through
    /// frameReturned() when it's ready.
    void needFrame(QProjector::Pattern *p);
    
public slots:


private:
    /// All cameras in use by the manager.
    /// Not necessarily all cameras attached to the system.
    std::vector<QCamera *> cameras;
    /// The projector we're using.
    QProjector *projector;
    /// The (planar) calibration standard we're using.
    CalibrationStandard *standard;

    /// Connect signals and slots in a camera to this manager.
    void connectCameraSignals(QCamera *cam);
    /// Disconnect signals and slots before removing the camera
    /// from the manager.
    void disconnectCameraSignals(QCamera *cam);

protected:
    /// Capture a frame from each camera.
    void captureFrame();
};

#endif // QSCANMANANGER_H
