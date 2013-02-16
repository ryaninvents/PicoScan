#include "calibrator.h"

#include "hardware/standards/calibrationstandard.h"
#include <opencv2/calib3d/calib3d.hpp>
#include <stdio.h>
#include "geom/triangulator.h"

Calibrator::Calibrator()
{

}

void Calibrator::setScanManager(QScanMananger *m)
{
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
    QCamera *left, *right;

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
    QCamera *camera;

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

bool Calibrator::addProjectorCalibrationFrame()
{
    QCamera *camera = manager->getFirst();
    CalibrationStandard *standard = manager->getStandard();

    cv::Mat frame;
    std::vector<cv::Point3f> objectPoints;
    std::vector<cv::Point2f> imagePoints;
    bool *success;
    uint x,y;

    cv::Mat rvec;
    // point on plane
    cv::Vec3d P;
    // rotation mtx for plane
    cv::Mat rot;
    // plane normal
    cv::Mat N;
    // in-plane vectors
    cv::Vec3d inPlane1, inPlane2;
    //
    std::vector<cv::Mat> frames;
/*
    // project white pattern and capture
    //manager->getScreen()->projectWhite();
    frame = camera->getFrameBW();

    // locate the standard
    imagePoints = standard->getImagePoints(frame,success);

    if(!(*success)) return false;

    objectPoints = standard->getObjectPoints();

    // calculate the plane eqn
    cv::solvePnP(objectPoints,
                 imagePoints,
                 camera->getIntrinsics(),
                 camera->getDistortion(),
                 rvec,
                 P);

    cv::Rodrigues(rvec,rot);

    // compute the plane's normal
    N = rot * cv::Mat(cv::Vec3d(0,0,1)).reshape(1);
    // compute the in-plane vectors
    // this only works if the standard is NOT facing directly up
    // (which, since up is defined by the camera's orientation,
    // is a little impossible; the camera would not be able to
    // resolve such a pose. thank you natural constraints!)
    inPlane1 = N.cross(cv::Vec3d(0,1,0));
    inPlane2 = N.cross(inPlane1);

    // capture full binary frame
    frames = manager->takeBinaryFrame();
    frame = frames.at(0);

    cv::Point3d A = objectPoints[manager->getStandard()->getPointA()];
    cv::Point3d B = objectPoints[manager->getStandard()->getPointB()];
    cv::Point3d C = objectPoints[manager->getStandard()->getPointC()];
    cv::Point3d D = objectPoints[manager->getStandard()->getPointD()];
    cv::Vec3d imPt;
    cv::Vec3d camRay;
    int code;

    for(x=0;x<frame.cols;x++){
        for(y=0;y<frame.rows;y++){
            imPt = cv::Vec3d(x,y,0);
            code = frame.at<int>(y,x);
            // ignore points not on board
            if( !(  Triangulator::inTri(imPt,A,B,C) ||
                    Triangulator::inTri(imPt,B,C,D)) || code<0) continue;

            // calculate 3D points by intersecting cam rays w/ std plane
            camRay = camera->getPixelRay(x,y);
            imPt = Triangulator::sumTo(camRay,inPlane1,inPlane2,P);
            projectorPoints.at(code).push_back(cv::Point3d(
                                                   imPt[0],
                                                   imPt[1],
                                                   imPt[2]
                                                   ));
        }
    }
*/
    return true;
}

// use least^2 for U_p planes
// Calc posn/orient/throw

