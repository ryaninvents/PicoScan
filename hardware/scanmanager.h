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

    /** Release all cameras from this manager. */
    void releaseAll();

    /** Get a binary frame from the system.
    \returns A vector of matrices. Each matrix in the vector
            corresponds to the \em nth camera.*/
    std::vector<cv::Mat> takeBinaryFrame();

    /** Set the stereo mode. */
    void setStereo(bool ster);

    /** Is this manager in stereo mode? */
    bool isStereo();

    /// Get the projection screen we're using.
    ProjectionScreen *getScreen();

    /// Set the projection screen.
    void setScreen(ProjectionScreen *screen){this->screen = screen;}

    /// Add a bit matrix to a given encoding matrix.
    static void addBinaryBit(cv::Mat encoding, cv::Mat img, cv::Mat inv, int bit);

    /// Decode a Gray-encoded matrix and apply a quality mask.
    static void decodeAndApplyMask(cv::Mat encoding, cv::Mat mask);

    /// Drop the @em n least significant bits from the input.
    static cv::Mat dropBits(cv::Mat input, unsigned int n);

    /** Set the lowest bit used for binary. Bits below this threshold
        will use fringe projection. */
    void setLowestBit(int n){lowestBit = n;}
    /** Get the lowest bit used for binary. */
    int getLowestBit(){return lowestBit;}
    /** Set the number of frames averaged together for binary shots. */
    void setAveragedFramesForBinary(unsigned int avg){avgFmsBinary = avg;}
    /** Get the number of frames averaged together for binary shots. */
    unsigned int getAveragedFramesForBinary(){return avgFmsBinary;}
    /** Set the number of frames averaged together for sinusoidal fringe shots. */
    void setAveragedFramesForFringes(unsigned int avg){avgFmsFringe = avg;}
    /** Get the number of frames averaged together for sinusoidal fringe shots. */
    unsigned int getAveragedFramesForFringes(){return avgFmsFringe;}

private:
    std::vector<Camera *> cameras;

    ProjectionScreen *screen;

    CalibrationStandard *standard;

    bool stereo;
    int lowestBit;
    unsigned int avgFmsBinary;
    unsigned int avgFmsFringe;
    int maxBright;

    std::vector<cv::Mat> takeBinaryStereoFrame();
    std::vector<cv::Mat> takeBinaryMonoFrame();
};

#endif // SCANMANAGER_H
