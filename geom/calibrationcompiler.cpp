#include "calibrationcompiler.h"
#include "hardware/projector/flatcolorpattern.h"
#include <opencv2/calib3d/calib3d.hpp>
#include <iostream>

CalibrationCompiler::CalibrationCompiler(QObject *parent) :
    QObject(parent),
    cameraLeft(0),
    cameraRight(0),
    projector(0),
    bin(0),
    waitingForLeft(false),
    waitingForRight(false),
    fail(false),
    _enabled(false)
{
}

void CalibrationCompiler::frameCaptured(
        cv::Mat frame,
        QCamera *cam,
        QProjector::Pattern *)
{
    emit debug(tr("(CC) frameCaptured();\n(CC) waitingForLeft = %1;\n(CC) waitingForRight = %2;")
               .arg(waitingForLeft).arg(waitingForRight));
    bool success;
    std::vector<cv::Point2f> imagePts;
    if(!waitingForLeft && !waitingForRight){
        echo("(CC) return from frameCaptured();");
        return;
    }
    if(cam==cameraLeft){
        emit debug(tr("(CC) cam==cameraLeft;"));
        imagePts = standard->getImagePoints(frame,&success);
        if(success){
            poisLeft.push_back(imagePts);
        }else{
            imagePts.clear();
            poisLeft.push_back(imagePts);
        }
        waitingForLeft = false;
        if(!success) fail = true;
    }else if(cam==cameraRight){
        emit debug(tr("(CC) cam==cameraRight;"));
        imagePts = standard->getImagePoints(frame,&success);
        if(success){
            poisRight.push_back(imagePts);
        }else{
            imagePts.clear();
            poisRight.push_back(imagePts);
        }
        waitingForRight = false;
        if(!success) fail = true;
    }else{
        echo("(CC) Unrecognized camera");
        return;
    }
    if(!(waitingForLeft||waitingForRight)){
        connectCameras(false);
        if(fail){
            emit debug(tr("(CC) fail;\n(CC) framesCaptured(%1);")
                       .arg(poisLeft.size()));
            poisLeft.pop_back();
            poisRight.pop_back();
            emit framesCaptured(poisLeft.size());
        }else{
            takeBinaryFrame();
        }
    }
}

void CalibrationCompiler::takeStereoFrame()
{
    emit debug(tr("(CC) takeStereoFrame();"));
    if(waitingForLeft||waitingForRight) return;
    waitingForLeft = true;
    waitingForRight = true;
    connectCameras(true);
    fail = false;
    FlatColorPattern *pattern = new FlatColorPattern();
    pattern->setID(poisLeft.size());
    projector->queue(pattern);

    emit debug(tr("(CC) waitingForLeft = %1;\n(CC) waitingForRight = %2;")
               .arg(waitingForLeft).arg(waitingForRight));
}

void CalibrationCompiler::takeBinaryFrame()
{
    emit debug(tr("(CC) takeBinaryFrame();"));
    if(bin){
        bin->requestFrame(11);
    }else{
        emit framesCaptured(poisLeft.size());
        emit debug(tr("(CC) No binary compiler"));
    }
}

void CalibrationCompiler::binaryFrameCaptured(cv::Mat frame, bool)
{
    emit debug(tr("(CC) binaryFrameCaptured(); framesCaptured(%1);")
               .arg(poisLeft.size()));
    emit framesCaptured(poisLeft.size());
}

double CalibrationCompiler::calibrate()
{

    std::vector<std::vector<cv::Point3f> > objectPoints;

    cv::Mat cameraL = cv::Mat::eye(3,3,CV_64F);
    cv::Mat distortionL = cv::Mat::zeros(8, 1, CV_64F);
    cv::Mat cameraR = cv::Mat::eye(3,3,CV_64F);
    cv::Mat distortionR = cv::Mat::zeros(8, 1, CV_64F);
    std::vector<cv::Mat> rvecsL, rvecsR;
    std::vector<cv::Mat> tvecsL, tvecsR;

    uint i;
    double rpeL;
    double rpeR;

    std::vector<cv::Point3f> cornersReal;
    std::vector<cv::Point2f> cornersImage;

    cv::Mat relativeRot,relativeTrans;

    objectPoints = standard->getObjectPoints(poisLeft.size());

    rpeL = cv::calibrateCamera(objectPoints,poisLeft,
                                      cameraLeft->getResolution(),
                                      cameraL,distortionL,rvecsL,tvecsL,
                                      CV_CALIB_FIX_ASPECT_RATIO|
                                      CV_CALIB_FIX_PRINCIPAL_POINT);
    rpeR = cv::calibrateCamera(objectPoints,poisRight,
                                      cameraRight->getResolution(),
                                      cameraR,distortionR,rvecsR,tvecsR,
                                      CV_CALIB_FIX_ASPECT_RATIO|
                                      CV_CALIB_FIX_PRINCIPAL_POINT);

    cameraLeft  -> setIntrinsics(cameraL);
    cameraLeft  -> setDistortion(distortionL);
    cameraRight -> setIntrinsics(cameraR);
    cameraRight -> setDistortion(distortionR);


    for(i=0;i<poisLeft.size();i++){
        cornersReal.push_back(cv::Point3f(tvecsL.at(i).at<double>(0),
                                          tvecsL.at(i).at<double>(1),
                                          tvecsL.at(i).at<double>(2)));
        cornersImage.push_back(poisRight[i][0]);
    }

    solvePnP(cornersReal,
             cornersImage,
             cameraR,
             distortionR,
             relativeRot,
             relativeTrans);

    cameraLeft  -> setPosition(cv::Vec3d());
    cameraRight -> setPosition(relativeTrans);
    cameraLeft  -> setOrientation(cv::Vec3d());
    cameraRight -> setOrientation(relativeRot);

    std::cout << cameraL << '\n'
              << cameraR << '\n'
              << distortionL << '\n'
              << distortionR << '\n'
              << relativeRot << '\n'
              << relativeTrans << '\n';

    return (rpeL+rpeR)/2;
}

void CalibrationCompiler::removeFrames()
{
    if(waitingForLeft||waitingForRight) return;
    poisLeft.clear();
    poisRight.clear();
    emit framesCaptured(0u);
}

void CalibrationCompiler::setLeft(QCamera *cam)
{
    cameraLeft = cam;
}

void CalibrationCompiler::setRight(QCamera *cam)
{
    cameraRight = cam;
}

void CalibrationCompiler::setBinary(BinaryCompiler *b)
{
    bin = b;
}

void CalibrationCompiler::setProjector(QProjector *pj)
{
    projector = pj;
}

void CalibrationCompiler::setStandard(CalibrationStandard *s)
{
    standard = s;
}

void CalibrationCompiler::setEnabled(bool enabled)
{
    _enabled = enabled;
    if(enabled){
        connect(bin,
                SIGNAL(binaryFrameCaptured(cv::Mat,bool)),
                this,
                SLOT(binaryFrameCaptured(cv::Mat,bool)));
    }else{
        bin->disconnect(this);
    }
}

void CalibrationCompiler::echo(QString s)
{
    emit debug(s);
}

void CalibrationCompiler::echo(const char *s)
{
    emit debug(tr(s));
}

void CalibrationCompiler::connectCameras(bool b)
{
    if(b){
        connect(cameraLeft,
                SIGNAL(frameCaptured(cv::Mat,QCamera*,QProjector::Pattern*)),
                this,
                SLOT(frameCaptured(cv::Mat,QCamera*,QProjector::Pattern*)));
        connect(cameraRight,
                SIGNAL(frameCaptured(cv::Mat,QCamera*,QProjector::Pattern*)),
                this,
                SLOT(frameCaptured(cv::Mat,QCamera*,QProjector::Pattern*)));

    }else{
        cameraLeft->disconnect(this);
        cameraRight->disconnect(this);
    }
}
