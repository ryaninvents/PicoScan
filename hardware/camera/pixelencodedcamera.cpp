#include "pixelencodedcamera.h"

PixelEncodedCamera::PixelEncodedCamera(QObject *parent) :
    QCamera(parent),
    inProgress(false)
{
}

void PixelEncodedCamera::setCapturingCamera(QCamera *cam)
{
    camera->disconnect(this);
    camera = cam;
    connect(camera,
            SIGNAL(frameCaptured(cv::Mat,FrameType)),
            this,
            SLOT(frameReturned(cv::Mat,FrameType)));
}

QCamera *PixelEncodedCamera::getCapturingCamera()
{
    return camera;
}

void PixelEncodedCamera::setProjector(QProjector *proj)
{
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

void PixelEncodedCamera::frameReturned(cv::Mat frame, QCamera::FrameType )
{
    setCapturedFrame(frameIdx,frame);
    dpcy->satisfy();
    compileFrames();
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
