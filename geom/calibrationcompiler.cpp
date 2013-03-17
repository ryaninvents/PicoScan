#include "calibrationcompiler.h"
#include "hardware/projector/flatcolorpattern.h"
CalibrationCompiler::CalibrationCompiler(QObject *parent) :
    QObject(parent),
    waitingForLeft(false),
    waitingForRight(false),
    fail(false)
{
}

void CalibrationCompiler::frameCaptured(
        cv::Mat frame,
        QCamera *cam,
        QProjector::Pattern *pattern)
{
    if(cam==cameraLeft){

    }else if(cam==cameraRight){

    }
}

void CalibrationCompiler::takeStereoFrame()
{
    if(waitingForLeft||waitingForRight) return;
    QProjector *projector = cameraLeft->getProjector();
    FlatColorPattern *pattern = new FlatColorPattern();
    pattern->setID(framesLeft.size());
    projector->queue(pattern);
    waitingForLeft = true;
    waitingForRight = true;
}
