#ifndef CALIBRATIONCOMPILER_H
#define CALIBRATIONCOMPILER_H

#include <QObject>
#include "hardware/camera/qcamera.h"
#include "hardware/standards/calibrationstandard.h"

/// Calibrates a stereo setup.
/// Only solves the cameras.

class CalibrationCompiler : public QObject
{
    Q_OBJECT
public:
    /// Create the compiler
    explicit CalibrationCompiler(QObject *parent = 0);

signals:
    /// Successful stereo-frame capture
    /// announce number of frames captured
    void framesCaptured(uint);
    /// Setup has been calibrated
    void calibrated(QCamera*, QCamera*, QProjector*);
    
public slots:
    /// One of the cameras has captured a frame
    void frameCaptured(cv::Mat frame,
                       QCamera *cam,
                       QProjector::Pattern *pattern);

    /// Take a stereo frame
    void takeStereoFrame();

private:
    /// POIs from left camera
    std::vector<std::vector<cv::Point3f> >
        framesLeft;
    /// POIs from right camera
    std::vector<std::vector<cv::Point3f> >
        framesRight;

    /// Left camera
    QCamera *cameraLeft;
    /// Right camera
    QCamera *cameraRight;

    /// Calibration standard
    CalibrationStandard *standard;

    /// Waiting for left image?
    bool waitingForLeft;
    /// Waiting for right image?
    bool waitingForRight;
    /// Failed to find a pattern?
    bool fail;
    
};

#endif // CALIBRATIONCOMPILER_H
