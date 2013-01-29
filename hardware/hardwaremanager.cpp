#include "hardwaremanager.h"

#include "camera/opencvcamera.h"

#define MAX_CAMERAS 3

HardwareManager::HardwareManager()
{
}

Camera *HardwareManager::getCamera(unsigned int i)
{
    if(i>=numCameras()) return 0;
    return cameras.at(i);
}

unsigned int HardwareManager::numCameras()
{
    return cameras.size();
}

unsigned int HardwareManager::refreshCameras()
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

void HardwareManager::releaseAll()
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
