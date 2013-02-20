#include "binarycamera.h"

BinaryCamera::BinaryCamera(QObject *parent) :
    QCamera(parent),
    threshold(20),
    bit(0),
    isWorking(false)
{
}

bool BinaryCamera::isOpen()
{
    return camera->isOpen();
}

bool BinaryCamera::requestFrame(QCamera::FrameType type)
{
    if(isWorking||type==QCamera::FULL_COLOR) return false;

}
