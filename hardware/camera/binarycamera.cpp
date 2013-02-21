#include "binarycamera.h"

#include <opencv2/highgui/highgui.hpp>


BinaryCamera::BinaryCamera(uint bit, QObject *parent):
    QCamera(parent),
    bit(bit),
    camera(0),
    projector(0),
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
    emit debug(tr("Someone has requested a frame from BinaryCamera %1")
               .arg(getBit()));
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

    emit debug(tr("BinaryCamera %3 recieved Gray code pattern,"
                  " bit %1, %2")
               .arg(gray->getBit())
               .arg(gray->isInverted()?
                        "inverted":
                        "not inverted")
               .arg(getBit()));

    // if it's the wrong bit, return
    if(gray->getBit()!=bit) return;

    emit debug(tr("Pattern accepted."));

    // connect to capture camera
    connectCaptureCamera();

    // tell the projector to wait; we're taking a pic
    projectorCanAdvance = false;

    // save the pattern
    this->pattern = gray;

    emit debug(tr("BinaryCamera %1 is requesting frame from capture camera.")
               .arg(getBit()));

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

    emit debug(tr("Capture camera for %1 returned a valid frame.")
               .arg(getBit()));

    disconnectCaptureCamera();

    // save the frame in the appropriate place
    if(!this->pattern->isInverted()){
        this->negative = frame;
        haveNegative = true;
        emit debug(tr("Frame is binary-negative."));
        cv::imwrite(tr("/home/ryan/binary-neg-%1.png")
                    .arg(getBit())
                    .toLocal8Bit().data()
                    ,negative);
    }else{
        this->positive = frame;
        havePositive = true;
        emit debug(tr("Frame is binary-positive."));
        cv::imwrite(tr("/home/ryan/binary-pos-%1.png")
                    .arg(getBit())
                    .toLocal8Bit().data()
                    ,positive);
    }

    // release the projector
    projectorCanAdvance = true;
    projector->requestAdvance();

    // can we calculate the binary frame?
    if(havePositive && haveNegative){
        emit debug(tr("Binary camera %1 has all required information.")
                   .arg(getBit()));
        // well then, calculate and emit it
        cv::Mat binary = positive - negative;
        emit frameCaptured(binary,
                           QCamera::DOUBLE,
                           this);
        emit debug(tr("Captured binary frame for bit %1 has been emitted.")
                   .arg(getBit()));
        cv::imwrite(tr("/home/ryan/binary-test-%1.png")
                    .arg(getBit())
                    .toLocal8Bit().data()
                    ,binary);
        isWorking = false;
    }
}

void BinaryCamera::disconnectCaptureCamera()
{
    emit debug(tr("BinaryCamera %1 disconnects from capture.")
               .arg(getBit()));
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
    emit debug(tr("BinaryCamera %1 connects to capture.")
               .arg(getBit()));
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
    emit debug(QString("BinaryCamera %1 recieves signal: projector is about to advance.")
               .arg(getBit()));
    emit debug(QString("BinaryCamera %2 tells projector %1 wait.")
               .arg(projectorCanAdvance?"not to":"that it must")
               .arg(getBit()));
    emit allowProjectorToAdvance(projectorCanAdvance);
}
