#include "phasecompiler.h"

#include "../hardware/projector/sinusoidpattern.h"
#include "triangulator.h"

PhaseCompiler::PhaseCompiler(QCamera *cam,
                             QObject *parent) :
    QObject(parent),
    camera(cam),
    horiz(false)
{
    connect(cam,
            SIGNAL(frameCaptured(cv::Mat,
                                 QCamera*,
                                 QProjector::Pattern*
                                 )),
            this,
            SLOT(frameCaptured(cv::Mat,
                               QCamera*,
                               QProjector::Pattern*
                               )));
}

void PhaseCompiler::requestFrame(uint width, uint shifts)
{
    uint n;
    frames.clear();
    captured.clear();
    frames.resize(shifts);
    captured.resize(shifts,false);
    for(n=0;n<shifts;n++){
        projector->queue(new SinusoidPattern(width,n));
    }
}

void PhaseCompiler::testAndEmit()
{
}

void PhaseCompiler::frameCaptured(cv::Mat frame, QCamera *cam, QProjector::Pattern *pattern)
{
}
