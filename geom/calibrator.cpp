#include "calibrator.h"

#include "hardware/standards/calibrationstandard.h"
#include <opencv2/calib3d/calib3d.hpp>
#include <stdio.h>

Calibrator::Calibrator()
{

}

void Calibrator::setScanManager(ScanManager *m)
{
    printf("Calibrator manager\t0x%x\n",m);
    manager = m;
}

void Calibrator::clearFrames()
{
    imagePointsLeft.clear();
    imagePointsRight.clear();
}

int Calibrator::addImagePair(cv::Mat left, cv::Mat right)
{
    CalibrationStandard *std = manager->getStandard();

    bool success;
    std::vector<cv::Point2f> leftPts, rightPts;

    leftPts = std->getImagePoints(left,&success);
    if(!success) return -1;

    rightPts = std->getImagePoints(right,&success);
    if(!success) return -1;

    if(imagePointsLeft.size()==0){
        leftSize = cv::Size(left.cols,left.rows);
        rightSize = cv::Size(right.cols,right.rows);
    }

    imagePointsLeft.push_back(leftPts);
    imagePointsRight.push_back(rightPts);
    return imagePointsLeft.size();
}

int Calibrator::addSingleImage(cv::Mat img)
{
    CalibrationStandard *std = manager->getStandard();

    bool success;
    std::vector<cv::Point2f> imgPts;

    imgPts = std->getImagePoints(img,&success);
    if(!success) return -1;

    if(imagePointsLeft.size()==0){
        leftSize = cv::Size(img.cols,img.rows);
    }

    imagePointsLeft.push_back(imgPts);
    return imagePointsLeft.size();
}

bool Calibrator::runCalibStereo()
{
    double rpeLeft, rpeRight;
    CalibrationStandard *std;
    cv::Mat leftMat, leftDist, rightMat, rightDist;
    std::vector<cv::Mat> leftRot, leftTrans, rightRot, rightTrans;
    Camera *left, *right;

    std::vector<cv::Point3d> cornersReal;
    std::vector<cv::Point2d> cornersImage;

    std::vector<std::vector<cv::Point3f> > objectPts;

    cv::Mat relativeRot, relativeTrans;

    unsigned int i;


    if(imagePointsLeft.size()<6) return false;

    std = manager->getStandard();

    left = manager->getFirst();
    right = manager->getSecond();

    leftMat = cv::Mat::eye(3,3,CV_32F);
    rightMat = cv::Mat::eye(3,3,CV_32F);

    leftDist = cv::Mat::zeros(8,1,CV_32F);
    rightDist = cv::Mat::zeros(8,1,CV_32F);

    objectPts = std->getObjectPoints(imagePointsLeft.size());

    printf("imagePointsLeft[%d]\nimagePointsLeft[0][%d]\n",imagePointsLeft.size(),imagePointsLeft.at(0).size());

    rpeLeft = cv::calibrateCamera(objectPts,
                                  imagePointsLeft,
                                  leftSize,
                                  leftMat,
                                  leftDist,
                                  leftRot,
                                  leftTrans,
                                  CV_CALIB_FIX_ASPECT_RATIO|
                                  CV_CALIB_FIX_PRINCIPAL_POINT);


    rpeRight = cv::calibrateCamera(objectPts,
                                   imagePointsRight,
                                   rightSize,
                                   rightMat,
                                   rightDist,
                                   rightRot,
                                   rightTrans,
                                   CV_CALIB_FIX_ASPECT_RATIO|
                                   CV_CALIB_FIX_PRINCIPAL_POINT);

    left->setIntrinsics(leftMat);
    left->setDistortion(leftDist);

    right->setIntrinsics(rightMat);
    right->setDistortion(rightDist);



    for(i=0;i<imagePointsRight.size();i++){
        cornersReal.push_back(cv::Point3d(
                                  leftTrans.at(i).at<double>(0),
                                  leftTrans.at(i).at<double>(1),
                                  leftTrans.at(i).at<double>(2)
                                  ));
        cornersImage.push_back(imagePointsRight.at(i).at(0));
    }

    cv::solvePnP(cornersReal,
                 cornersImage,
                 rightMat,
                 rightDist,
                 relativeRot,
                 relativeTrans);


    left->setPosition(cv::Vec3d());
    left->setOrientation(cv::Vec3d());

    right->setPosition(cv::Vec3d(
                           relativeTrans.at<double>(0),
                           relativeTrans.at<double>(1),
                           relativeTrans.at<double>(2)
                           ));

    right->setOrientation(cv::Vec3d(
                              relativeRot.at<double>(0),
                              relativeRot.at<double>(1),
                              relativeRot.at<double>(2)
                              ));

    return true;
}

bool Calibrator::runCalibMono()
{
    double rpe;
    CalibrationStandard *std;
    cv::Mat cameraMat, cameraDist;
    std::vector<cv::Mat> cameraRot, cameraTrans;
    Camera *camera;

    std::vector<std::vector<cv::Point3f> > objectPts;

    std = manager->getStandard();

    camera = manager->getFirst();

    cameraMat = cv::Mat::eye(3,3,CV_32F);

    cameraDist = cv::Mat::zeros(8,1,CV_32F);

    if(imagePointsLeft.size()<6) return false;

    objectPts = std->getObjectPoints(imagePointsLeft.size());

    rpe = cv::calibrateCamera(objectPts,
                                  imagePointsLeft,
                                  leftSize,
                                  cameraMat,
                                  cameraDist,
                                  cameraRot,
                                  cameraTrans,
                                  CV_CALIB_FIX_ASPECT_RATIO|
                                  CV_CALIB_FIX_PRINCIPAL_POINT);

    camera->setIntrinsics(cameraMat);
    camera->setDistortion(cameraDist);

    camera->setPosition(cv::Vec3d());
    camera->setOrientation(cv::Vec3d());

    return true;
}

