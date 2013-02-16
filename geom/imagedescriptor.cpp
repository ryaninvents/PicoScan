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
