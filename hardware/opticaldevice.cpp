#include "opticaldevice.h"

#include <opencv2/calib3d/calib3d.hpp>

OpticalDevice::OpticalDevice()
{
    orientation = cv::Mat::eye(3,3,CV_64F);
    position = cv::Vec3d();
    intrinsicMatrix = cv::Mat::eye(3,3,CV_64F);
    resolution = cv::Size(800,600);
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
    // rodrigues rotation matrix
    cv::Mat w, I;
    cv::Vec3d axis;
    I = cv::Mat::eye(3,3,CV_64F);
    double theta = sqrt(o.dot(o));
    axis[0] = o[0]/theta;
    axis[1] = o[1]/theta;
    axis[2] = o[2]/theta;
    cv::Rodrigues(axis,w);
    orientation = I + w*sin(theta) + w*w*(1-cos(theta));
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

void OpticalDevice::setFocalLength(double f)
{
    intrinsicMatrix.at<double>(0,0) = f;
    intrinsicMatrix.at<double>(1,1) = f;
}

void OpticalDevice::setPrincipalPoint(double u, double v)
{
    intrinsicMatrix.at<double>(0,2) = u;
    intrinsicMatrix.at<double>(1,2) = v;
}

double OpticalDevice::getNormalizedCoord(int u, int ww)
{
    double uprime = (double)(u) - (double)ww*0.5 + 0.5;
    return (uprime/getFocalLength());
}






