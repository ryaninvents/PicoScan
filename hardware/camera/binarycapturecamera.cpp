#include "binarycapturecamera.h"

#include "../projector/graycodepattern.h"

BinaryCaptureCamera::BinaryCaptureCamera(QObject *parent) :
    QCamera(parent),
    inProgress(false)
{
}

bool BinaryCaptureCamera::setBitRange(uint lo, uint hi)
{
    if(lo>=hi) return false;
    loBit = lo;
    hiBit = hi;
}

bool BinaryCaptureCamera::requestFrame(QCamera::FrameType type)
{
    // we can't handle more than one thing at a time
    // we also can't handle color
    if(inProgress || type==FULL_COLOR)
        return false;

    this->type = type;
    frames.resize((hiBit-loBit)*2);
}

void BinaryCaptureCamera::patternProjected(
        QProjector::Pattern *pattern)
{
    GrayCodePattern *gray;
    gray = dynamic_cast<GrayCodePattern*>(pattern);

    // if it's not Gray code we're not interested
    if(gray==0) return;

    uint bit = gray->getBit();

    // if the bit's out of range we're not interested
    // (boy we're picky)
    if(bit<loBit || bit>hiBit) return;

    currentFrameIndex = 2*(bit-loBit) + gray->isInverted()?1:0;

    // request a frame
    camera->requestFrame(UNSIGNED_16);
}
