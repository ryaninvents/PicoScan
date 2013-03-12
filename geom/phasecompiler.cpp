#include "phasecompiler.h"

PhaseCompiler::PhaseCompiler(QObject *parent) :
    QObject(parent)
{
}

void PhaseCompiler::requestFrame(uint width, uint shifts)
{
}

void PhaseCompiler::testAndEmit()
{
}

void PhaseCompiler::frameCaptured(cv::Mat frame, QCamera *cam, QProjector::Pattern *pattern)
{
}
