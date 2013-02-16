#ifndef IMAGEDESCRIPTOR_H
#define IMAGEDESCRIPTOR_H

/// Describes an image by identifying
/// the pattern projected as well as
/// the camera.
class ImageDescriptor
{
public:
    /// Create a new ImageDescriptor.
    ImageDescriptor(unsigned int camera,
                    unsigned int frame,
                    bool color);
    /// Get the camera ID.
    unsigned int getCameraID();
    /// Get the frame ID.
    unsigned int getFrameID();
    /// Is it a color image?
    bool isColor();
private:
    /// ID of the camera that took this image
    unsigned int cameraID;
    /// ID of the frame this represents
    unsigned int frameID;
    /// Is this a color image?
    bool color;
};

#endif // IMAGEDESCRIPTOR_H
