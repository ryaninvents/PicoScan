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
    uint i;

    // we can't handle more than one thing at a time
    // we also can't handle color
    if(inProgress || type==FULL_COLOR)
        return false;

    // do some bookkeeping
    inProgress = true;
    this->type = type;

    // resize the frames buffer
    frames.resize((hiBit-loBit)*2);

    // push the projection patterns to the projector
    // and null out the frame buffer
    for(i=loBit;i<=hiBit;i++){
        projector->queue(new GrayCodePattern(i,true));
        projector->queue(new GrayCodePattern(i,false));
        frames.at(2*(i-loBit)) = cv::Mat::zeros(1,1,CV_8U);
        frames.at(2*(i-loBit)+1) = cv::Mat::zeros(1,1,CV_8U);
    }

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
