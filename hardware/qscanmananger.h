#ifndef QSCANMANANGER_H
#define QSCANMANANGER_H

#include <QObject>
#include <opencv2/core/core.hpp>
#include "projector/qprojector.h"
#include "camera/qcamera.h"
#include "standards/calibrationstandard.h"
#include "../geom/uniqueimage.h"
#include "../image/imageprocessor.h"

/// Scan manager using signals and slots.
class QScanMananger : public QObject
{
    Q_OBJECT
public:
    /// Create a new ScanManager
    explicit QScanMananger(QObject *parent = 0);

    /// Fetch an image based on its camera ID and frame ID.
    /// The image has already been captured. To request
    /// frames, use needFrame().
    UniqueImage fetchCapturedImage(unsigned int camera,
                                   unsigned int frame);

    /// Set the projector to use.
    void setProjector(QProjector *p);

    /// Get the projector
    QProjector *getProjector();

    /// Get a specific camera
    QCamera *getCamera(uint i);

    /// Add a camera to the manager
    void setCamera(uint i, QCamera *camera);

    /// Count the cameras
    uint numCameras();
    
signals:
    /// Request a grayscale frame. Will be passed back in through
    /// frameReturned() when it's ready.
    void needFrame(QProjector::Pattern *p,
                   ImageDescriptor desc);
    
public slots:
    /// A frame has come back from the capturing system.
    void frameReturned(UniqueImage image);
    void requestFullFrame();

private:
    /// All cameras in use by the manager.
    /// Not necessarily all cameras attached to the system.
    std::vector<QCamera *> cameras;
    /// The projector we're using.
    QProjector *projector;
    /// The (planar) calibration standard we're using.
    CalibrationStandard *standard;
    /// The queue of patterns we wish to project.
    std::vector<QProjector::Pattern> patternQueue;
    /// The images we've collected so far
    std::vector<UniqueImage> images;
    /// Images we've requested but that are not back
    std::vector<ImageDescriptor> waiting;
    /// Images we still need
    std::vector<ImageDescriptor> neededFrames;
    /// The image processor that will handle images
    ImageProcessor *processor;

    /// Connect signals and slots in a camera to this manager.
    void connectCameraSignals(QCamera *cam);
    /// Disconnect signals and slots before removing the camera
    /// from the manager.
    void disconnectCameraSignals(QCamera *cam);

protected:
    /// Capture a frame from each camera.
    void captureFrame(ImageDescriptor desc);
};

#endif // QSCANMANANGER_H
