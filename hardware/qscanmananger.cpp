#include "qscanmananger.h"

QScanMananger::QScanMananger(QObject *parent) :
    QObject(parent),
    projector(0)
{
}

void QScanMananger::frameReturned(UniqueImage image){

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

uint QScanMananger::numCameras()
{
    return cameras.size();
}
