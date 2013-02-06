#include "scanmanager.h"

#include "camera/opencvcamera.h"
#include "hardware/projector/reflectedbinarypattern.h"
#include <opencv2/core/core.hpp>

#define MAX_CAMERAS 3


ScanManager::ScanManager():
    stereo(false)
{
    cameras = std::vector<Camera *>();
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

std::vector<cv::Mat> ScanManager::takeBinaryFrame()
{
    if(isStereo())
        return takeBinaryStereoFrame();
    else
        return takeBinaryMonoFrame();
}

void ScanManager::setStereo(bool ster)
{
    stereo = ster;
}

bool ScanManager::isStereo()
{
    return stereo && numCameras()>=2;
}

ProjectionScreen *ScanManager::getScreen()
{
    return screen;
}

std::vector<cv::Mat> ScanManager::takeBinaryStereoFrame()
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

    // return vector
    std::vector<cv::Mat> out;

    nmax = (uint) ceil(log(screen->size().width())/log(2));

    encodingLeft = cv::Mat::zeros(getFirst()->getResolutionV(),getFirst()->getResolutionU(),CV_32S);
    encodingRight = cv::Mat::zeros(getSecond()->getResolutionV(),getSecond()->getResolutionU(),CV_32S);

    screen->projectOnDisplay(1);

    for(n=0;n<nmax;n++){
//        pattern->setBit(n);
//        pattern->setInverted(false);
//        screen->displayPattern(pattern);
        screen->projectBinary(n,false);
        imgLeft = getFirst()->getFrameBW();
        imgRight = getSecond()->getFrameBW();

//        pattern->setInverted(true);
//        screen->displayPattern(pattern);
        screen->projectBinary(n,true);
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
            encodingLeft.at<unsigned int>(y,x) = screen->grayToBinary(
                        encodingLeft.at<unsigned int>(y,x));
        }
    }
    for(x=0;x<encodingRight.cols;x++){
        for(y=0;y<encodingRight.rows;y++){
            if(imgRight.at<int>(y,x) >0){
                encodingRight.at<unsigned int>(y,x) = screen->grayToBinary(
                        encodingRight.at<unsigned int>(y,x));
            }
        }
    }

    out.push_back(encodingLeft);
    out.push_back(encodingRight);

    return out;
}

std::vector<cv::Mat> ScanManager::takeBinaryMonoFrame()
{
    // number of binary bits needed to cover the whole image
    uint nmax;

    // current bit
    uint n;

    // for looping through image pixels
    uint x,y;

    // encoded frame
    cv::Mat encoding;

    // quality mask
    cv::Mat mask;

    // temporary frame holders
    cv::Mat img, inv;

    // return vector
    std::vector<cv::Mat> out;

    nmax = (uint) ceil(log(screen->size().width())/log(2));

    encoding = cv::Mat::zeros(getFirst()->getResolutionV(),getFirst()->getResolutionU(),CV_32S);

    screen->projectOnDisplay(1);

    for(n=0;n<nmax;n++){
        screen->projectBinary(n,false);
        img = getFirst()->getFrameBW();

        screen->projectBinary(n,true);
        inv = getFirst()->getFrameBW();

        img = img - inv;

        for(x=0;x<encoding.cols;x++){
            for(y=0;y<encoding.rows;y++){
                if(img.at<int>(y,x) >0){
                    encoding.at<unsigned int>(y,x) += 1<<n;
                }
            }
        }
    }


    for(x=0;x<encoding.cols;x++){
        for(y=0;y<encoding.rows;y++){
            encoding.at<unsigned int>(y,x) = screen->grayToBinary(
                        encoding.at<unsigned int>(y,x));
        }
    }

    out.push_back(encoding);

    return out;
}
