#ifndef SCANMANAGER_H
#define SCANMANAGER_H

#include "camera/camera.h"
#include <vector>

class ScanManager
{
public:
    ScanManager();
    /**
      Add a specific camera to the manager.
      */
    void addCamera(Camera * cam);
    /**
      Remove a camera from the manager.
      */
    void removeCamera(unsigned int i);
    /**
      Get a camera from the manager.
      */
    Camera *getCamera(unsigned int i);

    /** Count the number of cameras this manager is responsible for. */
    unsigned int numCameras();

    /** Have the system determine which camera to add to the manager. */
    bool addCamera();

private:
    std::vector<Camera *> cameras;
};

#endif // SCANMANAGER_H
