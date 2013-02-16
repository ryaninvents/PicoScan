#include "uniqueimage.h"

UniqueImage::UniqueImage(unsigned int camera,
                         unsigned int frame,
                         qint64 timestamp,
                         cv::Mat data):
    id(camera,frame,false),
    image(data),
    timestamp(timestamp)
{
}

UniqueImage::UniqueImage(ImageDescriptor desc,
                         qint64 timestamp,
                         cv::Mat data):
    id(desc),
    timestamp(timestamp),
    image(data)
{
}

unsigned int UniqueImage::getCameraID()
{
    return id.getCameraID();
}

unsigned int UniqueImage::getFrameID()
{
    return id.getFrameID();
}

cv::Mat UniqueImage::getImageData()
{
    return image;
}
