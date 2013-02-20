#include "binarycamera.h"

#include <opencv2/highgui/highgui.hpp>


BinaryCamera::BinaryCamera(uint bit, QObject *parent):
    QCamera(parent),
    bit(bit),
    camera(0),
    projecctor(0),
    isWorking(false),
    projectorCanAdvance(true)
{
}



BinaryCamera::BinaryCamera(
        QCamera *captureCamera,
        QProjector *projector,
        uint bit,
        QObject *parent) :
    QCamera(parent),
    bit(bit),
    isWorking(false),
    camera(captureCamera),
    projector(projector),
    projectorCanAdvance(true)
{
    connectProjector();
    connectCaptureCamera();
}

bool BinaryCamera::isOpen()
{
    return camera->isOpen();
}

void BinaryCamera::release()
{
    camera->release();
}

QString BinaryCamera::describe()
{
    return QString("Binary camera");
}

void BinaryCamera::setCaptureCamera(QCamera *capCam)
{
    disconnectCaptureCamera();
    camera = capCam;
    connectCaptureCamera();
}

void BinaryCamera::setProjector(QProjector *pj)
{
    disconnectProjector();
    projector = pj;
    connectProjector();
}

uint BinaryCamera::getBit()
{
    return bit;
}

bool BinaryCamera::requestFrame(QCamera::FrameType type)
{
    if(     isWorking
            || type==QCamera::FULL_COLOR
            || type==QCamera::UNSIGNED_16
            || camera==0
            || projector==0)
        return false;
    emit debug(tr("Someone has requested a frame from the BinaryCamera"));
    havePositive = false;
    haveNegative = false;
    isWorking = true;
    GrayCodePattern *pattern =
            new GrayCodePattern(bit,false);
    emit debug(tr("Queueing projector pattern, bit %1, not inverted")
               .arg(bit));
    projector->queue(pattern);
    pattern = new GrayCodePattern(bit,true);
    emit debug(tr("Queueing projector pattern, bit %1, not inverted")
               .arg(bit));
    projector->queue(pattern);
}

void BinaryCamera::patternProjected(
        QProjector::Pattern *p,
        QProjector *pj)
{
    // if it's not from the right projector, return
    if(pj!=projector) return;

    GrayCodePattern *gray =
            dynamic_cast<GrayCodePattern*>(p);

    // if it's not gray code, return
    if(gray==0) return;

    emit debug(tr("BinaryCamera recieved Gray code pattern,"
                  " bit %1, %2")
               .arg(gray->getBit())
               .arg(gray->isInverted()?
                        "inverted":
                        "not inverted"));

    // if it's the wrong bit, return
    if(gray->getBit()!=bit) return;

    // tell the projector to wait; we're taking a pic
    projectorCanAdvance = false;

    // save the pattern
    this->pattern = gray;

    emit debug(tr("BinaryCamera is requesting frame from capture camera."));

    // request a frame from the camera
    camera->requestFrame(QCamera::DOUBLE);

}

void BinaryCamera::rawFrameCaptured(cv::Mat frame,
                                    QCamera::FrameType type,
                                    QCamera *cam)
{
    // make sure the frame is B&W
    // and from our camera
    if(type==QCamera::FULL_COLOR||
            cam!=this->camera) return;

    emit debug(tr("Capture camera returned a valid frame."));

    // save the frame in the appropriate place
    if(this->pattern->isInverted()){
        this->negative = frame;
        haveNegative = true;
        emit debug(tr("Frame is binary-negative."));
    }else{
        this->positive = frame;
        havePositive = true;
        emit debug(tr("Frame is binary-positive."));
    }

    // release the projector
    projectorCanAdvance = true;
    projector->requestAdvance();

    // can we calculate the binary frame?
    if(havePositive && haveNegative){
        emit debug(tr("Binary camera has all required information."));
        // well then, calculate and emit it
        cv::Mat binary = positive - negative;
        emit frameCaptured(binary,
                           QCamera::DOUBLE,
                           this);
        emit debug(tr("Captured binary frame has been emitted."));
        cv::imwrite("/home/ryan/binary-test.png",binary);
        isWorking = false;
    }
}

void BinaryCamera::disconnectCaptureCamera()
{
    if(camera==0) return;
    camera->disconnect(this);
}

void BinaryCamera::disconnectProjector()
{
    if(projector==0) return;
    projector->disconnect(this);
    this->disconnect(projector);
}

void BinaryCamera::connectCaptureCamera()
{

    connect(camera,
            SIGNAL(frameCaptured(cv::Mat,QCamera::FrameType,QCamera*)),
            this,
            SLOT(rawFrameCaptured(cv::Mat,QCamera::FrameType,QCamera*)));
}

void BinaryCamera::connectProjector()
{

    connect(projector,
            SIGNAL(aboutToAdvance()),
            this,
            SLOT(projectorAboutToAdvance()));
    connect(this,
            SIGNAL(allowProjectorToAdvance(bool)),
            projector,
            SLOT(permissionToAdvance(bool)));
    connect(projector,
            SIGNAL(patternProjected(QProjector::Pattern*,QProjector*)),
            this,
            SLOT(patternProjected(QProjector::Pattern*,QProjector*)));
}

void BinaryCamera::projectorAboutToAdvance()
{
    emit debug(QString("BinaryCamera recieves signal: projector is about to advance."));
    emit debug(QString("BinaryCamera tells projector %1 wait.")
               .arg(projectorCanAdvance?"not to":"that it must"));
    emit allowProjectorToAdvance(projectorCanAdvance);
}
