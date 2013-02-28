#include "calibrator.h"

Calibrator::Calibrator(QObject *parent) :
    QObject(parent)
{
}

void Calibrator::frameCaptured(cv::Mat frame, QCamera *cam, QProjector::Pattern *pattern)
{
}
