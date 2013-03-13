#ifndef CALIBRATOR_H
#define CALIBRATOR_H

#include <QObject>
#include "../hardware/camera/qcamera.h"

/// Utility class for performing calibrations.
class Calibrator : public QObject
{
    Q_OBJECT
public:
    /// Create the calibrator.
    explicit Calibrator(QObject *parent = 0);

    /// Set the left camera
    void setLeft(QCamera *cam);
    /// Set the right camera
    void setRight(QCamera *cam);
    
signals:
    
public slots:
    /// One of our cameras has captured a frame
    void frameCaptured(cv::Mat frame,
                       QCamera *cam,
                       QProjector::Pattern *);
    /// Request a stereo frame
    void takeStereoFrame();

private:
    /// Camera on the left
    QCamera *leftCam;
    /// Camera on the right
    QCamera *rightCam;
    /// Frames captured from left camera
    std::vector<cv::Mat> framesLeft;
    /// Frames captured from right camera
    std::vector<cv::Mat> framesRight;
    
};

#endif // CALIBRATOR_H
