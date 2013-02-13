#include "opticaldevice.h"

#include <opencv2/calib3d/calib3d.hpp>
#include <iostream>

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

void OpticalDevice::setOrientation(cv::Vec3d r)
{
    /*// rodrigues rotation matrix
    cv::Mat w, I;
    cv::Vec3d axis;
    I = cv::Mat::eye(3,3,CV_64F);
    double theta = sqrt(r.dot(r));
    axis[0] = r[0]/theta;
    axis[1] = r[1]/theta;
    axis[2] = r[2]/theta;
    cv::Rodrigues(axis,w);
    orientation = I + w*sin(theta) + w*w*(1-cos(theta));*/
    cv::Rodrigues(r,orientation);
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

double OpticalDevice::getNormalizedU(double u)
{
    return getNormalizedCoord(u,getPrincipalU());
}

double OpticalDevice::getNormalizedV(double v)
{
    return getNormalizedCoord(v,getPrincipalV());
}

cv::Mat OpticalDevice::getOrientation()
{
    return orientation;
}

cv::Mat OpticalDevice::getPixelRay(double u, double v)
{
    // normalized U and V coordinates
    double uNorm, vNorm;

    // first we compute the ray in camera coordinates
    cv::Mat camBased = cv::Mat::ones(3,1,CV_64F);
    cv::Mat ray;

    // get the normalized coordinates
    uNorm = getNormalizedU(u);
    vNorm = getNormalizedV(v);

    // find the ray in camera coords
    camBased.at<double>(0) = uNorm;
    camBased.at<double>(1) = vNorm;

    // translate it based on the device's rotation
    // cv::normalize(orientation * camBased, ray);
    ray = orientation * camBased;

    return ray;

}

cv::Mat OpticalDevice::getUpVector()
{
    // first we define what "up" is
    cv::Vec3d upVec(0.0,-1.0,0.0);
    cv::Mat up;
    up = cv::Mat(upVec).reshape(1);

    //std::cout << up << "\n";

    // then we rotate it to match this device
    return orientation * up;
}

cv::Mat OpticalDevice::getFwdVector()
{
    // first we define what "forward" is
    cv::Vec3d fwdVec(0.0,0.0,1.0);
    cv::Mat fwd;
    fwd = cv::Mat(fwdVec).reshape(1);


    // then we rotate it to match this device
    fwd = orientation * fwd;
    //std::cout << "Forward: " << fwd << "\n";
    return fwd;
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

double OpticalDevice::getPrincipalU()
{
    return intrinsicMatrix.at<double>(0,2);
}

double OpticalDevice::getPrincipalV()
{
    return intrinsicMatrix.at<double>(1,2);
}

double OpticalDevice::getNormalizedCoord(double u, double c)
{
    return (u - c)/getFocalLength();
}






