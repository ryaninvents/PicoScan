#include "opticaldevice.h"

#include <opencv2/calib3d/calib3d.hpp>

OpticalDevice::OpticalDevice()
{
}

void OpticalDevice::setDistortion(cv::Mat k)
{
    distortion = k;
}

void OpticalDevice::setIntrinsics(cv::Mat m)
{
    intrinsicMatrix = m;
}

void OpticalDevice::setName(QString n)
{
    name = n;
}

void OpticalDevice::setOrientation(cv::Mat o)
{
    orientation = o;
}

void OpticalDevice::setOrientation(cv::Vec3d o)
{
    cv::Rodrigues(o,orientation);
}

void OpticalDevice::setPosition(cv::Vec3d p)
{
    position = p;
}

void OpticalDevice::setResolution(int u, int v)
{
    resolution = cv::Size(u,v);
}

void OpticalDevice::setResolution(cv::Size s)
{
    this->resolution = s;
}

cv::Mat OpticalDevice::getDistortion()
{
    return distortion;
}

cv::Mat OpticalDevice::getIntrinsics()
{
    return intrinsicMatrix;
}

QString OpticalDevice::getName()
{
    return name;
}

double OpticalDevice::getNormalizedU(int u)
{
    return getNormalizedCoord(u,getResolutionU());
}

double OpticalDevice::getNormalizedV(int v)
{
    return -getNormalizedCoord(v,getResolutionV());
}

cv::Mat OpticalDevice::getOrientation()
{
    return orientation;
}

cv::Mat OpticalDevice::getPixelRay(int u, int v)
{
    // normalized U and V coordinates
    double uNorm, vNorm;

    // first we compute the ray in camera coordinates
    cv::Matx31d camBased;

    // get the normalized coordinates
    uNorm = getNormalizedU(u);
    vNorm = getNormalizedV(v);

    // find the ray in camera coords
    camBased = cv::Matx31d(uNorm,vNorm,1);

    // translate it based on the device's rotation
    return orientation.mul(camBased);

}

cv::Mat OpticalDevice::getUpVector()
{
    // first we define what "up" is
    cv::Vec3d up(0.0,-1.0,0.0);

    // then we rotate it to match this device
    return orientation.mul(up);
}

cv::Vec3d OpticalDevice::getPosition()
{
    return position;
}

int OpticalDevice::getResolutionU()
{
    return resolution.width;
}

int OpticalDevice::getResolutionV()
{
    return resolution.height;
}

cv::Size OpticalDevice::getResolution()
{
    return resolution;
}

double OpticalDevice::getFocalLength()
{
    return intrinsicMatrix.at<double>(0,0);
}

double OpticalDevice::getNormalizedCoord(int u, int ww)
{
    double uprime = (double)(u) - (double)ww*0.5 + 0.5;
    return (uprime/getFocalLength());
}






