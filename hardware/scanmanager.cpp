#include "scanmanager.h"

#include "camera/opencvcamera.h"

ScanManager::ScanManager()
{
}

void ScanManager::addCamera(Camera *cam)
{
    cameras.push_back(cam);
}

void ScanManager::removeCamera(unsigned int i)
{
    if(i>=numCameras()) return;
    cameras.erase(cameras.begin()+i);
}

Camera *ScanManager::getCamera(unsigned int i)
{
    if(i>=numCameras()) return 0;
    return cameras.at(i);
}

unsigned int ScanManager::numCameras()
{
    return cameras.size();
}

bool ScanManager::addCamera()
{
    Camera *cam = new OpenCVCamera(numCameras());
    cam->setName("Camera");
    addCamera(cam);
}
