#include "binaryencodedcamera.h"

#include <opencv2/highgui/highgui.hpp>

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
        emit debug(tr("Sorry Charlie"));
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
        for(i=loBit;i<=hiBit;i++){
            emit debug(tr("Encoded camera inits camera %1.")
                       .arg(i));
            cam = new BinaryCamera(camera,
                                   projector,
                                   i);
            cam->setProjector(projector);
            cam->setCaptureCamera(camera);
            connect(cam,
                    SIGNAL(debug(QString)),
                    this,
                    SLOT(passDebug(QString)));
            connect(cam,
                    SIGNAL(frameCaptured(cv::Mat,QCamera::FrameType,QCamera*)),
                    this,
                    SLOT(rawFrameCaptured(cv::Mat,QCamera::FrameType,QCamera*)));
            subCameras.push_back(cam);

            cam->requestFrame(QCamera::SIGNED_32);
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
    emit debug(tr("Frame returned, bit %1")
               .arg(bcam->getBit()));
    for(y=0;y<encoding.rows;y++){
        for(x=0;x<encoding.cols;x++){
            if(frame.at<int>(y,x)>0){
                encoding.at<int>(y,x) +=
                        1 << bcam->getBit();
            }
        }
    }
    reporting++;
    emit debug(tr("%1 bits reporting.")
               .arg(reporting));
    if(reporting>(hiBit-loBit)){
        decodeAndEmit();
    }
}

void BinaryEncodedCamera::passDebug(QString s)
{
    emit debug(s);
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
//    cv::Mat decoded = cv::Mat::zeros(
//                encoding.rows,
//                encoding.cols,
//                CV_32S);
    encoding.convertTo(encoding,CV_32S);
    for(y=0;y<encoding.rows;y++){
        for(x=0;x<encoding.cols;x++){
            encoding.at<int>(y,x) =
                    grayToBinary(
                        encoding.at<int>(y,x))
                    %256
                    ;
        }
    }
    emit frameCaptured(encoding,
                       QCamera::SIGNED_32,
                       this);
    cv::imwrite("/home/ryan/binary-full.png",encoding);
    emit debug(tr("=== Full binary frame emitted ==="));
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

uint BinaryEncodedCamera::grayToBinary(uint num)
{
    int numBits = 8 * sizeof(num);
    int shift;
    for (shift = 1; shift < numBits; shift = 2 * shift)
    {
        num = num ^ (num >> shift);
    }
    return num;
}

void BinaryEncodedCamera::setCaptureCamera(QCamera *cap)
{
    camera = cap;
}

void BinaryEncodedCamera::setProjector(QProjector *pj)
{
    projector = pj;
}
