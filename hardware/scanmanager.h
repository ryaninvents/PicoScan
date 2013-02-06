#ifndef SCANMANAGER_H
#define SCANMANAGER_H

#include "camera/camera.h"
#include <vector>
#include "standards/calibrationstandard.h"
#include "hardware/projector/projectionscreen.h"


/**
  A class to wrangle the cameras and projector.

  In the interest of simplicity, ScanManager automatically finds all cameras
  on the system.
  */
class ScanManager
{
public:
    ScanManager();

    /**
      Get a camera from the manager.
      */
    Camera *getCamera(unsigned int i);

    /** Determine whether left and right cameras are ready. */
    bool areCamerasSet();

    /** Count the number of cameras this manager is responsible for. */
    unsigned int numCameras();

    /** Refresh the list of cameras. */
    unsigned int getAllCameras();

    /** Assign a standard. */
    void setStandard(CalibrationStandard *std){standard = std;}

    /** Set the left camera. */
    void setFirst(Camera *cam);
    /** Set the right camera. */
    void setSecond(Camera *cam);

    /** Get the left camera. */
    Camera *getFirst(){return cameras.at(0);}
    /** Get the right camera. */
    Camera *getSecond(){return cameras.at(1);}

    /** Get the current standard. */
    CalibrationStandard *getStandard(){return standard;}

    void releaseAll();

    std::vector<cv::Mat> takeBinaryFrame();

    /** Set the stereo mode. */
    void setStereo(bool ster);

    /** Is this manager in stereo mode? */
    bool isStereo();

private:
    std::vector<Camera *> cameras;

    ProjectionScreen *screen;

    CalibrationStandard *standard;

    bool stereo;

    std::vector<cv::Mat> takeBinaryStereoFrame();
    std::vector<cv::Mat> takeBinaryMonoFrame();
};

#endif // SCANMANAGER_H
