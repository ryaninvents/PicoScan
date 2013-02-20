#ifndef BINARYENCODEDCAMERA_H
#define BINARYENCODEDCAMERA_H

#include "qcamera.h"
#include "binarycamera.h"
#include <vector>

/// A camera capable of taking several binary
/// frames and compiling them into an encoded
/// image.
class BinaryEncodedCamera : public QCamera
{
    Q_OBJECT
public:
    /// Create a new binary-encoded camera.
    explicit BinaryEncodedCamera(
            uint loBit,
            uint hiBit,
            QObject *parent = 0);

    /// Is the camera available to take frames?
    bool isOpen();

    /// Release the camera
    void release();

    /// Describe this camera
    QString describe();

    /// Get horizontal resolution
    int getResolutionU();

    /// Get vertical resolution
    int getResolutionV();

    /// Get overall resolution
    cv::Size getResolution();

    /// Set resolution
    void setResolution(cv::Size s);

    /// set resolution
    void setResolution(int u, int v);

    /// Convert Gray code to binary.
    static int grayToBinary(int num);
    
signals:
    
public slots:

    /// Capture a frame and emit when ready.
    bool requestFrame(QCamera::FrameType type);

    /// A child camera has returned a picture.
    void rawFrameCaptured(cv::Mat frame,
                          QCamera::FrameType type,
                          QCamera *cam);

private:

    /// The lowest bit this camera will capture.
    uint loBit;

    /// The highest bit this camera will capture.
    uint hiBit;

    /// Whether or not this camera is currently busy.
    bool isWorking;

    /// Individual binary cameras
    std::vector<BinaryCamera*> subCameras;

    /// Actual capture camera
    QCamera *camera;

    /// Projector
    QProjector *projector;

    /// Is this camera one of ours?
    bool isSubcam(QCamera *cam);

    /// Encoded frame so far
    cv::Mat encoding;

    /// How many cameras have reported back
    uint reporting;

    /// Decode and emit frame
    void decodeAndEmit();

    /// The type of image we want to emit
    QCamera::FrameType capType;
    
};

#endif // BINARYENCODEDCAMERA_H
