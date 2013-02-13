#include "scanmanager.h"

#include "camera/opencvcamera.h"
#include "hardware/projector/reflectedbinarypattern.h"
#include <opencv2/core/core.hpp>

#define MAX_CAMERAS 3


ScanManager::ScanManager():
    stereo(false),
    lowestBit(0),
    avgFmsBinary(3),
    avgFmsFringe(5)
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

void ScanManager::addBinaryBit(cv::Mat encoding, cv::Mat img, cv::Mat inv, int bit)
{
    unsigned int y,x;
    cv::Mat i2 = img - inv;
    for(x=0;x<encoding.cols;x++){
        for(y=0;y<encoding.rows;y++){
            if(i2.at<int>(y,x) >0){
                encoding.at<int>(y,x) += 1<<bit;
            }
        }
    }
}

cv::Mat ScanManager::decodeAndApplyMask(cv::Mat encoding, cv::Mat mask)
{
    unsigned int x,y;
    for(x=0;x<encoding.cols;x++){
        for(y=0;y<encoding.rows;y++){
            if(mask.at<int>(y,x) >0){
                encoding.at<int>(y,x) = ProjectionScreen::grayToBinary(
                        encoding.at<int>(y,x));
            }else {
                encoding.at<int>(y,x) = -1;
            }
        }
    }
}

cv::Mat ScanManager::dropBits(cv::Mat input, unsigned int n)
{
    cv::Mat output = cv::Mat::zeros(input.rows,input.cols,CV_32S);
    unsigned int x,y,mask;
    mask = ~((1<<n)-1);
    for(x=0;x<input.cols;x++){
        for(y=0;y<input.rows;y++){
            output.at<int>(y,x) = input.at<int>(y,x) & mask;
        }
    }
    return output;
}



std::vector<cv::Mat> ScanManager::takeBinaryStereoFrame()
{
    // number of binary bits needed to cover the whole image
    uint nmax;

    // current bit
    uint n;

    // encoded frame
    cv::Mat encodingFirst,encodingSecond;

    // quality mask
    cv::Mat maskFirst, maskSecond;

    // temporary frame holders
    cv::Mat imgFirst, invFirst, imgSecond, invSecond;

    // return vector
    std::vector<cv::Mat> out;

    // compute how many bits we need to uniquely ID each projector pixel
    nmax = (uint) ceil(log(screen->size().width())/log(2));

    // create matrices for the encoding and zero them out
    encodingFirst = cv::Mat::zeros(
                getFirst()->getResolutionV(),
                getFirst()->getResolutionU(),CV_32S);
    encodingSecond = cv::Mat::zeros(
                getSecond()->getResolutionV(),
                getSecond()->getResolutionU(),CV_32S);

    // turn on the projector
    screen->projectOnDisplay(1);

    // set camera modes
    getFirst()->setMode(BINARY_CAPTURE);
    getSecond()->setMode(BINARY_CAPTURE);

    // loop through bit values
    for(n=lowestBit;n<nmax;n++){
        // project binary pattern
        screen->projectBinary(n,false);
        imgFirst = getFirst()->getFrameBW(avgFmsBinary);
        imgSecond = getSecond()->getFrameBW(avgFmsBinary);

        // project inverted binary pattern
        screen->projectBinary(n,true);
        invFirst = getFirst()->getFrameBW(avgFmsBinary);
        invSecond = getSecond()->getFrameBW(avgFmsBinary);

        // add the binary bits to the image
        addBinaryBit(encodingFirst,imgFirst,invFirst,n);
        addBinaryBit(encodingSecond,imgSecond,invSecond,n);
    }


    // project white and capture
    screen->projectBinary(nmax+1,false);
    maskFirst = getFirst()->getFrameBW(avgFmsBinary);
    maskSecond = getSecond()->getFrameBW(avgFmsBinary);

    // project black and capture
    screen->projectBinary(nmax+1,true);
    maskFirst = maskFirst - getFirst()->getFrameBW(avgFmsBinary);
    maskSecond = maskSecond - getSecond()->getFrameBW(avgFmsBinary);


    // decode, apply mask, and push onto stack
    out.push_back(decodeAndApplyMask(encodingFirst,maskFirst));
    out.push_back(decodeAndApplyMask(encodingSecond,maskSecond));

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

    // set camera mode
    getFirst()->setMode(BINARY_CAPTURE);

    // compute how many bits we need to uniquely ID each projector pixel
    nmax = (uint) ceil(log(screen->size().width())/log(2));

    // create a matrix for encoding and zero it out
    encoding = cv::Mat::zeros(
                getFirst()->getResolutionV(),
                getFirst()->getResolutionU(),CV_32S);

    // make sure the projector is working
    screen->projectOnDisplay(1);

    // loop through bit values
    for(n=lowestBit;n<nmax;n++){
        // project binary pattern
        screen->projectBinary(n,false);
        img = getFirst()->getFrameBW(avgFmsBinary);

        // project inverted binary pattern
        screen->projectBinary(n,true);
        inv = getFirst()->getFrameBW(avgFmsBinary);

        // add the bit to the image
        addBinaryBit(encoding,img,inv,n);
    }

    // project white and capture
    screen->projectBinary(nmax+1,false);
    mask = getFirst()->getFrameBW(avgFmsBinary);

    // project black and capture
    screen->projectBinary(nmax+1,true);
    mask = mask - getFirst()->getFrameBW(avgFmsBinary);

    // decode, apply mask, push
    out.push_back(decodeAndApplyMask(encoding,mask));

    return out;
}
