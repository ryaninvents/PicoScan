#include "imagedescriptor.h"

ImageDescriptor::ImageDescriptor(unsigned int camera,
                                 unsigned int frame,
                                 bool color):
    cameraID(camera),
    frameID(frame),
    color(color)
{
}

unsigned int ImageDescriptor::getCameraID()
{
    return cameraID;
}

unsigned int ImageDescriptor::getFrameID()
{
    return frameID;
}

bool ImageDescriptor::isColor()
{
    return color;
}

unsigned int ImageDescriptor::getInfoFromID(
        unsigned int position,
        unsigned int size)
{
    unsigned int mask;
    unsigned int raw = getFrameID();
    raw = raw >> position;
    mask = (1U << size)-1;
    return raw & mask;
}
