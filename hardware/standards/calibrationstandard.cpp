#include "calibrationstandard.h"

#include <opencv2/calib3d/calib3d.hpp>

CalibrationStandard::CalibrationStandard()
{
}

std::vector<std::vector<cv::Point3d> > CalibrationStandard::getObjectPoints(
        unsigned int n)
{
    std::vector<std::vector<cv::Point3d> > out;
    unsigned int i;
    std::vector<cv::Point3d> pts = getObjectPoints();

    for(i=0;i<n;i++){
        out.push_back(pts);
    }
    return out;
}

std::vector<std::vector<cv::Point2d> > CalibrationStandard::getImagePoints(
        std::vector<cv::Mat> images,
        bool *success)
{
    std::vector<std::vector<cv::Point2d> > out;
    std::vector<cv::Point2d> tmp;
    unsigned int i;
    bool b;

    for(i=0;i<images.size();i++){
        tmp = this->getImagePoints(images.at(i),&b);
        if(!b){
            *success = false;
            return out;
        }
        out.push_back(tmp);
    }

    *success = true;

    return out;
}

bool CalibrationStandard::calibrate(Camera *camera,
                                    std::vector<cv::Mat> images,
                                    std::vector<cv::Mat> rvecs,
                                    std::vector<cv::Mat> tvecs,
                                    double *rpe)
{
    bool *found = false;
    std::vector<std::vector<cv::Point2d> > imagePoints;
    std::vector<std::vector<cv::Point3d> > objectPoints;
    cv::Mat cameraMatrix;
    cv::Mat distortionCoeffs;

    cameraMatrix = cv::Mat::eye(3,3,CV_64F);

    // search for the standard in every image
    imagePoints = getImagePoints(images, found);

    // we must not have found the standard
    if(!*found){
        return false;
    }

    objectPoints = getObjectPoints(images.size());

    *rpe = cv::calibrateCamera(objectPoints,
                              imagePoints,
                              cv::Size(images[0].size[0],
                                       images[0].size[1]),
                              cameraMatrix,
                              distortionCoeffs,
                              rvecs,
                              tvecs,
                              CV_CALIB_FIX_ASPECT_RATIO);

    camera->setIntrinsics(cameraMatrix);
    camera->setDistortion(distortionCoeffs);

    return true;
}
