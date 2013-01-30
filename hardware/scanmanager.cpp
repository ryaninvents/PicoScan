#include "scanmanager.h"

#include "camera/opencvcamera.h"

#define MAX_CAMERAS 3

ScanManager::ScanManager()
{
    cameras.push_back(0);
    cameras.push_back(0);
}

Camera *ScanManager::getCamera(unsigned int i)
{
    if(i>=numCameras()) return 0;
    return cameras.at(i);
}

bool ScanManager::areCamerasSet()
{
    return false;
}

unsigned int ScanManager::numCameras()
{
    return cameras.size();
}

unsigned int ScanManager::getAllCameras()
{
    unsigned int i;
    Camera *cam;

    releaseAll();

    for(i=0;i<MAX_CAMERAS;i++){
        cam = new OpenCVCamera(i);
        if(cam->isOpen()){
            cameras.push_back(cam);
        }else{
            return i;
        }
    }
    return MAX_CAMERAS;
}

void ScanManager::setLeft(Camera *cam)
{
    cameras.at(0) = cam;
}

void ScanManager::setRight(Camera *cam)
{
    cameras.at(1) = cam;
}

void ScanManager::releaseAll()
{
    uint i;
    Camera *cam;
    for(i=0;i<numCameras();i++){
        cam = cameras.at(i);
        cam->release();
        delete cam;
    }
    cameras.clear();
}
