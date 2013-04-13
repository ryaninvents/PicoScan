#include "binarycompiler.h"

#include "../hardware/projector/graycodepattern.h"
#include "triangulator.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

BinaryCompiler::BinaryCompiler(QCamera *cam,
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

void BinaryCompiler::requestFrame(uint nmax)
{
    uint n;
    nmax++;
    frames.clear();
    captured.clear();
    frames.resize(nmax*2);
    captured.resize(nmax*2,false);
    for(n=0;n<nmax;n++){
        projector->queue(new GrayCodePattern(n,false,horiz));
        projector->queue(new GrayCodePattern(n,true,horiz));
    }
}

void BinaryCompiler::setProjector(QProjector *pj)
{
    projector = pj;
}

void BinaryCompiler::frameCaptured(
        cv::Mat frame,
        QCamera *cam,
        QProjector::Pattern *pattern)
{
    GrayCodePattern *gray =
            dynamic_cast<GrayCodePattern*>(pattern);
    uint idx;
    if(cam!=camera || !gray) {
        return;
    }
    idx = (gray->getBit()*2) + (gray->isInverted()?1:0);
    cv::cvtColor(frame,frames.at(idx),CV_BGR2GRAY);
    captured.at(idx) = true;
    QString fnm = tr("/home/ryan/mqp-data/shots/binary-%1-%2.png")
            .arg(gray->getBit()).arg(gray->isInverted());
    cv::imwrite(fnm.toLocal8Bit().data(),frame);
    testAndEmit();
}

void BinaryCompiler::testAndEmit()
{
    uint n, nmax;
    cv::Mat encoding = cv::Mat::zeros(
                frames[0].rows,
                frames[0].cols,
                CV_32S);
    nmax = frames.size()/2;
    for(n=0;n<captured.size();n++){
        if(!captured.at(n)){
            return;
        }
    }
    encoding = Triangulator::computeBinary(
                frames,30);
    emit binaryFrameCaptured(encoding,horiz);
    cv::Mat color = Triangulator::maphsv(encoding,
                                         1<<nmax);
    emit visualBinaryFrame(color);
    cv::imwrite("/home/ryan/nubinary.png",color);
}
