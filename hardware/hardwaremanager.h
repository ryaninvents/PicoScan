#ifndef HARDWAREMANAGER_H
#define HARDWAREMANAGER_H

#include "camera/camera.h"
#include <vector>

/**
  A class to wrangle the cameras and projector.

  In the interest of simplicity, ScanManager automatically finds all cameras
  on the system.
  */
class HardwareManager
{
public:
    HardwareManager();

    /**
      Get a camera from the manager.
      */
    Camera *getCamera(unsigned int i);

    /** Count the number of cameras this manager is responsible for. */
    unsigned int numCameras();

    /** Refresh the list of cameras. */
    unsigned int refreshCameras();

    void releaseAll();

private:
    std::vector<Camera *> cameras;
};

#endif // SCANMANAGER_H
