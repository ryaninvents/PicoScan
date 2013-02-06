#include "scanmanager.h"

#include "camera/opencvcamera.h"
#include "hardware/projector/reflectedbinarypattern.h"
#include <opencv2/core/core.hpp>

#define MAX_CAMERAS 3


ScanManager::ScanManager()
{
    cameras.push_back(0);
    cameras.push_back(0);
}

Camera *ScanManager::getCamera(unsigned int i)
{
    if(i>=numCameras()) return 0;
    return cameras.at(i);
}

bool ScanManager::areCamerasSet()
{
    return false;
}

unsigned int ScanManager::numCameras()
{
    return cameras.size();
}

unsigned int ScanManager::getAllCameras()
{
    unsigned int i;
    Camera *cam;

    releaseAll();

    for(i=0;i<MAX_CAMERAS;i++){
        cam = new OpenCVCamera(i);
        if(cam->isOpen()){
            cameras.push_back(cam);
        }else{
            return i;
        }
    }
    return MAX_CAMERAS;
}

void ScanManager::setFirst(Camera *cam)
{
    cameras.at(0) = cam;
}

void ScanManager::setSecond(Camera *cam)
{
    cameras.at(1) = cam;
}

void ScanManager::releaseAll()
{
    uint i;
    Camera *cam;
    for(i=0;i<numCameras();i++){
        cam = cameras.at(i);
        cam->release();
        delete cam;
    }
    cameras.clear();
}

std::vector<cv::Mat> ScanManager::takeFrame()
{
    // number of binary bits needed to cover the whole image
    uint nmax;

    // current bit
    uint n;

    // for looping through image pixels
    uint x,y;

    // encoded frame
    cv::Mat encodingLeft,encodingRight;

    // quality mask
    cv::Mat mask;

    // temporary frame holders
    cv::Mat imgLeft, invLeft, imgRight, invRight;

    // pattern
    ReflectedBinaryPattern *pattern = new ReflectedBinaryPattern(
                screen->size().width(),0,false);

    // return vector
    std::vector<cv::Mat> out;

    nmax = (uint) ceil(log(screen->size().width())/log(2));

    encodingLeft = cv::Mat::zeros(getFirst()->getResolutionV(),getFirst()->getResolutionU(),CV_32S);
    encodingRight = cv::Mat::zeros(getSecond()->getResolutionV(),getSecond()->getResolutionU(),CV_32S);

    screen->projectOnDisplay(1);

    for(n=0;n<nmax;n++){
        pattern->setBit(n);

        pattern->setInverted(false);
        screen->displayPattern(pattern);
        imgLeft = getFirst()->getFrameBW();
        imgRight = getSecond()->getFrameBW();

        pattern->setInverted(true);
        screen->displayPattern(pattern);
        invLeft = getFirst()->getFrameBW();
        invRight = getSecond()->getFrameBW();

        imgLeft = imgLeft - invLeft;
        imgRight = imgRight - invRight;

        for(x=0;x<encodingLeft.cols;x++){
            for(y=0;y<encodingLeft.rows;y++){
                if(imgLeft.at<int>(y,x) >0){
                    encodingLeft.at<unsigned int>(y,x) += 1<<n;
                }
            }
        }
        for(x=0;x<encodingRight.cols;x++){
            for(y=0;y<encodingRight.rows;y++){
                if(imgRight.at<int>(y,x) >0){
                    encodingRight.at<unsigned int>(y,x) += 1<<n;
                }
            }
        }
    }


    for(x=0;x<encodingLeft.cols;x++){
        for(y=0;y<encodingLeft.rows;y++){
            encodingLeft.at<unsigned int>(y,x) = pattern->grayToBinary(
                        encodingLeft.at<unsigned int>(y,x));
        }
    }
    for(x=0;x<encodingRight.cols;x++){
        for(y=0;y<encodingRight.rows;y++){
            if(imgRight.at<int>(y,x) >0){
                encodingRight.at<unsigned int>(y,x) = pattern->grayToBinary(
                        encodingRight.at<unsigned int>(y,x));
            }
        }
    }

    out.push_back(encodingLeft);
    out.push_back(encodingRight);

    return out;
}

void ScanManager::setStereo(bool stereo)
{
    this->stereo = stereo;
}

bool ScanManager::isStereo()
{
    return stereo && numCameras()>=2;
}
