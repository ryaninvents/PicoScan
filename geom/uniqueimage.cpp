#include "uniqueimage.h"

UniqueImage::UniqueImage(unsigned int camera,
                         unsigned int frame,
                         cv::Mat data):
    cameraID(camera),
    frameID(frame),
    image(data)
{
}

unsigned int UniqueImage::getCameraID()
{
    return cameraID;
}

unsigned int UniqueImage::getFrameID()
{
    return frameID;
}

cv::Mat UniqueImage::getImageData()
{
    return image;
}
