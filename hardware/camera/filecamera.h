#ifndef FILECAMERA_H
#define FILECAMERA_H

#include "camera.h"

class FileCamera : public Camera
{
public:
    /// Create a new FileCamera, using the passed directory as the
    /// image source. Files are of the form [fnm][enum string][number].png
    FileCamera(char *finm);

    /// Set the camera mode
    void setMode(CameraMode m);

    /// Get a color frame.
    cv::Mat getFrame();

    cv::Mat getFrameBW();

    /// Is the camera open?
    bool isOpen();

    /// Release the camera's resources.
    /// Since there's no hardware involved, no problem bro!
    void release(){}

private:
    /// filename pattern
    char *fnm;

    /// index of current file
    int idx;

    /// Get a string describing the enum value.
    /// Used in generating filenames.
    char *getEnumValueAsString();

    /// Get the next filename.
    char *getNextFilename();
};

#endif // FILECAMERA_H
