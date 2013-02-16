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
    
signals:
    /// Request a grayscale frame. Will be passed back in through
    /// frameReturned() when it's ready.
    void needFrame(QProjector::Pattern *p, uint frameID);
    
public slots:
    /// A frame has come back from the capturing system.
    void frameReturned(cv::Mat frame,
                       qint64 timestamp,
                       uint frameID,
                       uint cameraID);

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
    
};

#endif // QSCANMANANGER_H
