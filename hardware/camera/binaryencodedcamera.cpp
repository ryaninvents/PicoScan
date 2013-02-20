#include "binaryencodedcamera.h"

BinaryEncodedCamera::BinaryEncodedCamera(uint loBit, uint hiBit, QObject *parent) :
    QCamera(parent),
    isWorking(false),
    camera(0),
    projector(0),
    loBit(loBit),
    hiBit(hiBit)
{
    if(loBit==hiBit){
        hiBit++;
    }else if(loBit<hiBit){
        uint tmp = hiBit;
        hiBit = loBit;
        loBit = tmp;
    }
}

bool BinaryEncodedCamera::isOpen()
{
    if(!camera) return false;
    return camera->isOpen();
}

void BinaryEncodedCamera::release()
{
    if(camera) camera->release();
}

QString BinaryEncodedCamera::describe()
{
    return tr("Binary-encoded camera ranging from"
              " %1 to %2 bits")
            .arg(loBit)
            .arg(hiBit);
}

int BinaryEncodedCamera::getResolutionU()
{
    return camera->getResolutionU();
}

int BinaryEncodedCamera::getResolutionV()
{
    return camera->getResolutionV();
}

bool BinaryEncodedCamera::requestFrame(QCamera::FrameType type)
{
    if(        !camera // if we don't have a camera
            || !projector // or if we don't have a projector
            || isWorking  // or if we're already busy
            || type==QCamera::FULL_COLOR  // or if we want color
            || type==QCamera::UNSIGNED_16 // or if we want u16
               ){
        // sorry Charlie
        return false;
    }else{
        uint i;
        BinaryCamera *cam;
        subCameras.clear();
        reporting = 0;
        encoding = cv::Mat::zeros(
                    getResolutionV(),
                    getResolutionU(),
                    CV_32S);
        for(i=0;(loBit+i)<=hiBit;i++){
            cam = new BinaryCamera(camera,
                                   projector,
                                   loBit+i);
            subCameras.push_back(cam);
        }
    }
}

void BinaryEncodedCamera::rawFrameCaptured(
        cv::Mat frame,
        QCamera::FrameType type,
        QCamera *cam)
{
    BinaryCamera *bcam;
    uint x,y;
    if(!isSubcam(cam)||type==QCamera::FULL_COLOR) return;
    bcam = dynamic_cast<BinaryCamera*>(cam);
    frame.convertTo(frame,CV_32S);
    for(y=0;y<encoding.rows;y++){
        for(x=0;x<encoding.cols;x++){
            if(frame.at<int>(y,x)>0)
                encoding.at<int>(y,x) +=
                        1 << bcam->getBit();
        }
    }
    reporting++;
    if(reporting>(hiBit-loBit)){
        decodeAndEmit();
    }
}

bool BinaryEncodedCamera::isSubcam(QCamera *cam)
{
    uint i;
    for(i=0;i<subCameras.size();i++){
        if(subCameras.at(i)==cam) return true;
    }
    return false;
}

void BinaryEncodedCamera::decodeAndEmit()
{
    uint x,y;
    cv::Mat decoded = cv::Mat::zeros(
                encoding.rows,
                encoding.cols,
                CV_32S);
    for(y=0;y<encoding.rows;y++){
        for(x=0;x<encoding.cols;x++){
            if(frame.at<int>(y,x)>0)
                decoded.at<int>(y,x) =
                        grayToBinary(
                            encoding.at<int>(y,x));
        }
    }
    decoded.convertTo(decoded,
                      getOpenCVFlagFromType(capType));
    emit frameCaptured(decoded,
                       capType,
                       this);
}

cv::Size BinaryEncodedCamera::getResolution()
{
    return camera->getResolution();
}

void BinaryEncodedCamera::setResolution(cv::Size s)
{
    camera->setResolution(s);
}

void BinaryEncodedCamera::setResolution(int u, int v)
{
    camera->setResolution(u,v);
}

int BinaryEncodedCamera::grayToBinary(int num)
{
    int numBits = 8 * sizeof(num);
    int shift;
    for (shift = 1; shift < numBits; shift = 2 * shift)
    {
        num = num ^ (num >> shift);
    }
    return num;
}
