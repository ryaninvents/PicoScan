#ifndef UNIQUEIMAGE_H
#define UNIQUEIMAGE_H

#include <opencv2/core/core.hpp>

/// Represents an image taken with specific intent by a
/// specific camera.
class UniqueImage
{
public:
    UniqueImage(unsigned int camera, unsigned int frame,
                cv::Mat data);

    unsigned int getCameraID();
    unsigned int getFrameID();
    cv::Mat getImageData();

private:
    /// ID of the camera that took this image
    unsigned int cameraID;
    /// ID of the frame this represents
    unsigned int frameID;
    /// Actual image data
    cv::Mat image;
};

#endif // UNIQUEIMAGE_H
