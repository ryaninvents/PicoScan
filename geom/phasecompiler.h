#ifndef PHASECOMPILER_H
#define PHASECOMPILER_H

#include <QObject>
#include "hardware/camera/qcamera.h"

/// Generates a sinusoidal phase map upon request

class PhaseCompiler : public QObject
{
    Q_OBJECT
public:
    /// Create the compiler
    explicit PhaseCompiler(QCamera *cam = 0, QObject *parent = 0);
    /// Request a frame
    /// \param width fringe width in px
    /// \param shifts number of (evenly-spaced) phase shifts
    void requestFrame(uint width, uint shifts);

    void setProjector(QProjector *proj);
    
signals:
    /// Phase mapping has been captured
    void phaseMapCaptured(cv::Mat, bool);
    /// Phase mapping has been captured; here's a
    /// pretty version for you to look at
    void visualPhaseMap(cv::Mat);
    /// Debug
    void debug(QString);
    
public slots:
    /// The camera has captured a frame
    void frameCaptured(cv::Mat frame,
                       QCamera *cam,
                       QProjector::Pattern *pattern);


private:
    /// Frames captured so far
    std::vector<cv::Mat> frames;

    /// Whether or not each frame has been captured
    std::vector<bool> captured;

    std::vector<int> fCt;
    int loops;

    /// Our camera
    QCamera *camera;

    /// Horizontal or vertical?
    bool horiz;

    /// Fringe width, px
    uint fringeSize;

    /// Test for completion; if complete, emit
    void testAndEmit();

    QProjector *projector;

};

#endif // PHASECOMPILER_H
