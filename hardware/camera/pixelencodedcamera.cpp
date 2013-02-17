#include "pixelencodedcamera.h"

PixelEncodedCamera::PixelEncodedCamera(QObject *parent) :
    QCamera(parent)
{
}

void PixelEncodedCamera::setCapturingCamera(QCamera *cam)
{
    camera = cam;
}

QCamera *PixelEncodedCamera::getCapturingCamera()
{
    return camera;
}

void PixelEncodedCamera::setProjector(QProjector *proj)
{
    projector = proj;
}

QProjector *PixelEncodedCamera::getProjector()
{
    return projector;
}
