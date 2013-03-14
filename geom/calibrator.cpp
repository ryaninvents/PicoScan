#include "calibrator.h"
#include <opencv2/calib3d/calib3d.hpp>

Calibrator::Calibrator(QObject *parent) :
    QObject(parent)
{
}

void Calibrator::setLeft(QCamera *cam)
{
    leftCam = cam;
}

void Calibrator::setRight(QCamera *cam)
{
    rightCam = cam;
}

double Calibrator::calibrate(CalibrationStandard *standard)
{

    std::vector<std::vector<cv::Point3f> > objectPoints;
    std::vector<std::vector<cv::Point2f> > imagePointsL,imagePointsR;

    cv::Mat cameraL = cv::Mat::eye(3,3,CV_64F);
    cv::Mat distortionL = cv::Mat::zeros(8, 1, CV_64F);
    cv::Mat cameraR = cv::Mat::eye(3,3,CV_64F);
    cv::Mat distortionR = cv::Mat::zeros(8, 1, CV_64F);
    std::vector<cv::Mat> rvecsL, rvecsR;
    std::vector<cv::Mat> tvecsL, tvecsR;


    std::vector<cv::Point3f> cornersReal;
    std::vector<cv::Point2f> cornersImage;

    cv::Mat relativeRot,relativeTrans;

    bool foundL = false, foundR = false;

    objectPoints = standard->getObjectPoints(framesLeft.size());
    imagePointsL = standard->getImagePoints(framesLeft,&foundL);
    imagePointsR = standard->getImagePoints(framesRight,&foundR);

    if(!foundL || !foundR){
        std::cout << foundL << "|" << foundR;
        return;
    }

    double rpe0 = cv::calibrateCamera(objectPoints,imagePointsL,
                                      cv::Size(framesLeft[0].size[0],framesLeft[1].size[1]),
                                      cameraL,distortionL,rvecsL,tvecsL,
                                      CV_CALIB_FIX_ASPECT_RATIO|
                                      CV_CALIB_FIX_PRINCIPAL_POINT);
    double rpe1 = cv::calibrateCamera(objectPoints,imagePointsR,
                                      cv::Size(framesRight[0].size[0],framesRight[1].size[1]),
                                      cameraR,distortionR,rvecsR,tvecsR,
                                      CV_CALIB_FIX_ASPECT_RATIO|
                                      CV_CALIB_FIX_PRINCIPAL_POINT);

    leftCam  -> setIntrinsics(cameraL);
    leftCam  -> setDistortion(distortionL);
    rightCam -> setIntrinsics(cameraR);
    rightCam -> setDistortion(distortionR);

    for(unsigned int i=0;i<framesLeft.size();i++){
        cornersReal.push_back(cv::Point3f(tvecsL.at(i).at<double>(0),
                                          tvecsL.at(i).at<double>(1),
                                          tvecsL.at(i).at<double>(2)));
        cornersImage.push_back(imagePointsR[i][0]);
    }

    solvePnP(cornersReal,
             cornersImage,
             cameraR,
             distortionR,
             relativeRot,
             relativeTrans);

    leftCam  -> setPosition(cv::Vec3d());
    rightCam -> setPosition(relativeTrans);
    leftCam  -> setOrientation(cv::Vec3d());
    rightCam -> setOrientation(relativeRot);

    return (rpe0+rpe1)/2;

}

void Calibrator::frameCaptured(cv::Mat frame, QCamera *cam, QProjector::Pattern *)
{
    if(cam==leftCam){
        framesLeft.push_back(frame);
        leftCam->disconnect(this);
    }else if(cam==rightCam){
        framesRight.push_back(frame);
        rightCam->disconnect(this);
    }
    countCalibrationFrames();
}

void Calibrator::takeStereoFrame()
{
    if(!leftCam || !rightCam) return;
    connect(leftCam,
            SIGNAL(frameCaptured(cv::Mat,QCamera*,QProjector::Pattern*)),
            this,
            SLOT(frameCaptured(cv::Mat,QCamera*,QProjector::Pattern*)));
    connect(rightCam,
            SIGNAL(frameCaptured(cv::Mat,QCamera*,QProjector::Pattern*)),
            this,
            SLOT(frameCaptured(cv::Mat,QCamera*,QProjector::Pattern*)));
    leftCam->requestFrame();
    rightCam->requestFrame();

}

void Calibrator::countCalibrationFrames()
{
    emit countFrame(framesLeft.size()>framesRight.size()?
                        framesRight.size():framesLeft.size());
}
