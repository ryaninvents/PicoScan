#include "phasecompiler.h"

#include "../hardware/projector/sinusoidpattern.h"
#include "triangulator.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

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
    int q;
    frames.clear();
    captured.clear();
    fCt.clear();
    frames.resize(shifts);
    captured.resize(shifts,false);
    fCt.resize(shifts,0);
    loops = 4;
    this->width = width;
    SinusoidPattern *s;
    for(q=0;q<loops;q++){
        for(n=0;n<shifts;n++){
            s = new SinusoidPattern(width,n*width/shifts);
            s->setID(n);
            s->setBrightness(150);
            projector->queue(s);
        }
    }
}

void PhaseCompiler::setProjector(QProjector *proj)
{
    this->projector = proj;
}

void PhaseCompiler::testAndEmit()
{
    uint n;
    cv::Mat phase;
    for(n=0;n<captured.size();n++){
        if(!captured.at(n)) return;
    }
    for(n=0;n<frames.size();n++){
        frames.at(n) /= loops;
    }
    phase = Triangulator::computePhase(frames,20,width);
    cv::Mat inLivingColor = Triangulator::maphsv(phase*1000,1000*width);
    cv::imwrite("/home/ryan/phase.png",inLivingColor);
    emit phaseMapCaptured(phase,false);
}

void PhaseCompiler::frameCaptured(cv::Mat frame, QCamera *cam, QProjector::Pattern *pattern)
{
    SinusoidPattern *sine = dynamic_cast<SinusoidPattern*>(pattern);
    if(cam!=camera || !sine){
        return;
    }
    cv::Mat fm;
    cv::cvtColor(frame,fm,CV_BGR2GRAY);
    if(fCt.at(sine->getID())==0)
        frames.at(sine->getID()) = fm;
    else
        frames.at(sine->getID()) = frames.at(sine->getID()) + fm;
    fCt.at(sine->getID())++;
    if(fCt.at(sine->getID())>=loops) captured.at(sine->getID()) = true;
    testAndEmit();
}
