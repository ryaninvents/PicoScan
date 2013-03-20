#ifndef BINARYCOMPILER_H
#define BINARYCOMPILER_H

#include <QObject>
#include "../hardware/camera/qcamera.h"

/// Generates a binary frame upon request.

class BinaryCompiler : public QObject
{
    Q_OBJECT
public:
    /// Create the compiler
    explicit BinaryCompiler(QCamera *cam,
                            QObject *parent = 0);

    /// Request a frame
    /// \param nmax number of binary bits
    void requestFrame(uint nmax);
    void setProjector(QProjector *pj);
    
signals:
    /// Binary frame has been captured
    void binaryFrameCaptured(cv::Mat,bool);
    /// Binary frame has been captured; here's a
    /// pretty version for you to look at
    void visualBinaryFrame(cv::Mat);
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

    /// Our camera
    QCamera *camera;
    /// Our projector
    QProjector *projector;

    /// Horizontal or vertical?
    bool horiz;
    
    /// Test for completion; if complete, emit
    void testAndEmit();

};

#endif // BINARYCOMPILER_H
