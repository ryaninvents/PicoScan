#ifndef UNIQUEIMAGE_H
#define UNIQUEIMAGE_H

#include <QObject>
#include <opencv2/core/core.hpp>
#include "imagedescriptor.h"

/// Represents an image taken with specific intent by a
/// specific camera.
class UniqueImage
{
public:
    /// Create a new UniqueImage
    UniqueImage(unsigned int camera,
                unsigned int frame,
                qint64 timestamp,
                cv::Mat data);

    /// Create a new UniqueImage
    UniqueImage(ImageDescriptor desc,
                qint64 timestamp,
                cv::Mat data);

    /// Get the camera ID
    unsigned int getCameraID();
    /// Get the frame ID
    unsigned int getFrameID();
    /// Get the image data
    cv::Mat getImageData();

private:
    /// Identify the image
    ImageDescriptor id;
    /// Actual image data
    cv::Mat image;
    /// Timestamp
    qint64 timestamp;
};

#endif // UNIQUEIMAGE_H
