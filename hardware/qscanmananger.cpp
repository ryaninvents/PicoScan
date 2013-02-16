#include "qscanmananger.h"

QScanMananger::QScanMananger(QObject *parent) :
    QObject(parent),
    projector(0)
{
}

UniqueImage QScanMananger::fetchCapturedImage(unsigned int camera, unsigned int frame)
{
}

void QScanMananger::frameReturned(UniqueImage image){

}

void QScanMananger::requestFullFrame()
{
    neededFrames = processor->getRequiredFrames();
    // captureFrame();
}

void QScanMananger::connectCameraSignals(QCamera *cam)
{
    connect(cam,SIGNAL(frameCaptured(UniqueImage)),
            this,SLOT(frameReturned(UniqueImage)));
}

void QScanMananger::disconnectCameraSignals(QCamera *cam)
{
    cam->disconnect(this);
}

void QScanMananger::captureFrame(ImageDescriptor desc)
{
    // figure out what pattern we need based on descriptor
    /// emit needFrame()
}

void QScanMananger::setProjector(QProjector *p)
{
    uint i;
    // disconnect the old projector, if there is one
    if(projector != 0){
        projector->disconnect(
                    SIGNAL(needFrame(QProjector::Pattern*,
                                     ImageDescriptor)));
    }
    // set the projector
    projector = p;
    // connect the projector to each camera
    for(i=0;i<numCameras();i++){
        projector->connect(
                    projector,
                    SIGNAL(patternProjected(ImageDescriptor)),
                    getCamera(i),
                    SLOT(requestFrame(ImageDescriptor))
                    );
    }
}

QProjector *QScanMananger::getProjector()
{
    return projector;
}

void QScanMananger::setStandard(CalibrationStandard *std)
{
    standard = std;
}

CalibrationStandard *QScanMananger::getStandard()
{
    return standard;
}

QCamera *QScanMananger::getCamera(uint i)
{
    return cameras.at(i);
}

void QScanMananger::setCamera(uint i, QCamera *camera)
{
    connectCameraSignals(camera);
    if(i>=numCameras()){
        cameras.push_back(camera);
    }else{
        disconnectCameraSignals(getCamera(i));
        cameras.at(i) = camera;
    }
}

uint QScanMananger::numCameras()
{
    return cameras.size();
}

void QScanMananger::limitNumCameras(int n)
{
    while(numCameras()>n) cameras.pop_back();
}

QCamera *QScanMananger::getFirst()
{
    return getCamera(0);
}

QCamera *QScanMananger::getSecond()
{
    return getCamera(1);
}

bool QScanMananger::isStereo()
{
    return numCameras()>1;
}
