#include "binarycamera.h"

BinaryCamera::BinaryCamera(uint bit, QObject *parent) :
    QCamera(parent),
    threshold(20),
    bit(bit),
    isWorking(false)
{
}

bool BinaryCamera::isOpen()
{
    return camera->isOpen();
}

QString BinaryCamera::describe()
{
    return QString("Binary camera");
}

bool BinaryCamera::requestFrame(QCamera::FrameType type)
{
    if(     isWorking
            ||type==QCamera::FULL_COLOR
            ||type==QCamera::UNSIGNED_16)
        return false;
    havePositive = false;
    haveNegative = false;
    isWorking = true;
    GrayCodePattern *pattern =
            new GrayCodePattern(bit,false);
    projector->queue(pattern);
    pattern = new GrayCodePattern(bit,true);
    projector->queue(pattern);
}

void BinaryCamera::patternProjected(
        QProjector::Pattern *p,
        QProjector *pj)
{
    // if it's not from the right projector, return
    if(pj!=projector) return;

    GrayCodePattern *gray =
            dynamic_cast<GrayCodePattern*>(p);

    // if it's not gray code, return
    if(gray==0) return;

    // if it's the wrong bit, return
    if(gray->getBit()!=bit) return;

    // tell the projector to wait; we're taking a pic
    projector->waitFor(this,SLOT(patternCaptured()));

    // save the pattern
    this->pattern = gray;

    // request a frame from the camera
    camera->requestFrame(QCamera::DOUBLE);

}

void BinaryCamera::frameCaptured(cv::Mat frame,
                                 QCamera::FrameType type,
                                 QCamera *cam)
{
    // make sure the frame is B&W
    // and from our camera
    if(type!=QCamera::DOUBLE||
            cam!=this->camera) return;

    // save the frame in the appropriate place
    if(this->pattern->isInverted()){
        this->negative = frame;
        haveNegative = true;
    }else{
        this->positive = frame;
        havePositive = true;
    }

    // release the projector
    projector->stopWaitingFor(this);

    // can we calculate the binary frame?
    if(havePositive && haveNegative){
        // well then, calculate and emit it
        cv::Mat binary = positive - negative;
        emit frameCaptured(binary,
                           QCamera::DOUBLE,
                           this);
        isWorking = false;
    }
}

void BinaryCamera::patternCaptured()
{
}
