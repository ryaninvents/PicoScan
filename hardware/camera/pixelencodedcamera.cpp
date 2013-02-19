#include "pixelencodedcamera.h"

#include <opencv2/core/core.hpp>

PixelEncodedCamera::PixelEncodedCamera(QObject *parent) :
    QCamera(parent),
    inProgress(false),
    camera(0),
    projector(0),
    dpcy(new ProjectorDependent)
{

}

void PixelEncodedCamera::setCapturingCamera(QCamera *cam)
{
    if(camera!=0){
        camera->disconnect(this);
    }
    camera = dynamic_cast<QCamera*>(cam);
    if(camera!=0){
        connect(camera,
                SIGNAL(frameCaptured(cv::Mat,QCamera::FrameType)),
                this,
                SLOT(frameReturned(cv::Mat,QCamera::FrameType)));
        if(projector!=0){
            connect(projector,
                    SIGNAL(patternProjected(QProjector::Pattern*)),
                    camera,
                    SLOT());
        }
    }
}

QCamera *PixelEncodedCamera::getCapturingCamera()
{
    return camera;
}

void PixelEncodedCamera::setProjector(QProjector *proj)
{
    if(projector!=0)
        projector->deregisterDependencies();
    projector = proj;
    projector->registerDependency(dpcy);
}

QProjector *PixelEncodedCamera::getProjector()
{
    return projector;
}

bool PixelEncodedCamera::isScanInProgress()
{
    return inProgress;
}

QCamera::FrameType PixelEncodedCamera::getFrameType()
{
    return requestedType;
}

bool PixelEncodedCamera::isOpen()
{
    return camera->isOpen();
}

void PixelEncodedCamera::release()
{
    camera->release();
}

void PixelEncodedCamera::frameReturned(
        cv::Mat frame,
        QCamera::FrameType )
{
    setCapturedFrame(frameIdx,frame);
    dpcy->satisfy();
    compileFrames();
}

void PixelEncodedCamera::patternProjected(QProjector::Pattern *pattern)
{
    // get the pattern's index
    int patternIndex = getPatternIndex(pattern);
    // if the pattern index is <0 then it's not
    // one of ours
    if(patternIndex<0) return;
    // remember which pattern we're waiting for
    waitingFor((uint)patternIndex);
    // request a frame
    camera->requestFrame(QCamera::DOUBLE);
}

void PixelEncodedCamera::allocateFrames(uint n)
{
    uint i;
    frames.resize(n);
    hasBeenCap.resize(n,false);
    for(i=0;i<n;i++){
        frames.at(i) = cv::Mat::zeros(1,1,CV_8U);
        hasBeenCap.at(i) = false;
    }
}

cv::Mat PixelEncodedCamera::getCapturedFrame(uint idx)
{
    return frames.at(idx);
}

void PixelEncodedCamera::setCapturedFrame(uint idx, cv::Mat frame)
{
    frames.at(idx) = frame;
}

void PixelEncodedCamera::clearCapturedFrames()
{
    uint size = frames.size();
    frames.clear();
    hasBeenCap.clear();
    allocateFrames(size);
}

bool PixelEncodedCamera::hasCapturedFrame(uint i)
{
    return hasBeenCap.at(i);
}

bool PixelEncodedCamera::capturedAllFrames()
{
    uint i;
    for(i=0;i<frames.size();i++){
        if(!hasCapturedFrame(i)) return false;
    }
    return true;
}

uint PixelEncodedCamera::framesAllocated()
{
    return frames.size();
}

void PixelEncodedCamera::waitingFor(uint frame)
{
    frameIdx = frame;
}
