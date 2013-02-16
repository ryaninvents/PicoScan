#include "qscanmananger.h"

QScanMananger::QScanMananger(QObject *parent) :
    QObject(parent),
    projector(0)
{
}

void QScanMananger::frameReturned(UniqueImage image){

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
