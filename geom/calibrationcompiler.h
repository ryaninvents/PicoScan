#ifndef CALIBRATIONCOMPILER_H
#define CALIBRATIONCOMPILER_H

#include <QObject>
#include "hardware/camera/qcamera.h"
#include "hardware/standards/calibrationstandard.h"
#include "geom/binarycompiler.h"

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
                       QProjector::Pattern *);

    /// Take a stereo frame
    void takeStereoFrame();

    /// Take a binary frame to calibrate the projector
    void takeBinaryFrame();

    /// Calibrate the setup
    double calibrate();

    /// Clear frames from memory
    void removeFrames();

    void setLeft(QCamera *cam);
    void setRight(QCamera *cam);
    void setBinary(BinaryCompiler *b);
    void setProjector(QProjector *pj);
    void setStandard(CalibrationStandard *s);

private:
    /// POIs from left camera
    std::vector<std::vector<cv::Point2f> >
        poisLeft;
    /// POIs from right camera
    std::vector<std::vector<cv::Point2f> >
        poisRight;

    /// Left camera
    QCamera *cameraLeft;
    /// Right camera
    QCamera *cameraRight;
    /// Projector
    QProjector *projector;

    /// Calibration standard
    CalibrationStandard *standard;

    /// Binary compiler.
    BinaryCompiler *bin;

    /// Waiting for left image?
    bool waitingForLeft;
    /// Waiting for right image?
    bool waitingForRight;
    /// Failed to find a pattern?
    bool fail;



};

#endif // CALIBRATIONCOMPILER_H
