#ifndef CAMERA_H
#define CAMERA_H

#include "../opticaldevice.h"
#include <opencv2/core/core.hpp>

enum CameraMode{
    CAMERA_CALIBRATION,
    PROJECTOR_CALIBRATION,
    BINARY_CAPTURE,
    FRINGE_CAPTURE
};

/**
  Represents a generic camera. The specifics
  of obtaining an image are left up to the
  implementation of child classes.
  */
class Camera: public OpticalDevice
{
public:
    /// Instantiate a camera.
    Camera();

    /** Get a frame from the camera. */
    virtual cv::Mat getFrame() = 0;

    /// Get a black-and-white frame.
    cv::Mat getFrameBW();

    /// Average several frames together.
    cv::Mat getFrameBW(unsigned int n);

    /** Are we waiting for a snapshot from
        the camera? */
    bool isWaiting(){ return waiting; }

    /** Notify the camera that we will be
        expecting an image in the near future.*/
    void notify(){ waiting = true; }

    /// Is the camera available to take frames?
    virtual bool isOpen() = 0;

    /// Set the camera mode
    void setMode(CameraMode m){mode = m;}

    /// Get the current camera mode
    CameraMode getMode(){return mode;}

    /// Release any resources this camera is using.
    virtual void release() = 0;

    /// Set the frame rate.
    void setFrameRate(int fps){}
private:
    /** Whether or not we are waiting for the
        camera to take a snapshot (since it
        may be synced with the projector). */
    bool waiting;

    /// Current mode
    CameraMode mode;
};

#endif // CAMERA_H
