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
    frames.resize((hiBit-loBit)*2+2);

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

bool BinaryCaptureCamera::hasCapturedRawFrame(uint bit, bool inv)
{
    if(bit<loBit || bit>hiBit) return false;
    uint idx = (bit-loBit)*2+(inv?1:0);
    return (frames.at(idx).rows>1 && frames.at(idx).cols>1);
}

cv::Mat BinaryCaptureCamera::getRawFrame(uint bit, bool inv)
{
    uint idx = (bit-loBit)*2+(inv?1:0);
    return (frames.at(idx));
}

cv::Mat BinaryCaptureCamera::compileFrames()
{
    uint bit,x,y;
    cv::Mat encoding = cv::Mat::zeros(
                getResolutionU(),
                getResolutionV(),
                CV_32S);
    cv::Mat img;
    for(bit=loBit;bit<=hiBit;bit++){
        if(!(hasCapturedRawFrame(bit,true)&&
             hasCapturedRawFrame(bit,false)))
            continue;
        img = getRawFrame(bit,false) - getRawFrame(bit,true);
        img.convertTo(img,CV_32S);
        for(x=0;x<img.cols;x++){
            for(y=0;y<img.rows;y++){
                if(img.at<int>(y,x)>0){
                    encoding.at<int>(y,x) += 1<<bit;
                }
            }
        }
    }
    encoding.convertTo(encoding,getOpenCVFlagFromType(type));
    emit intermediateFrame(encoding);
}
