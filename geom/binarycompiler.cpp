#include "binarycompiler.h"

#include "../hardware/projector/graycodepattern.h"
#include "triangulator.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

BinaryCompiler::BinaryCompiler(QCamera *cam,
                               QObject *parent) :
    QObject(parent),
    camera(cam)
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
    QProjector *projector = camera->getProjector();
    nmax++;
    frames.clear();
    captured.clear();
    frames.resize(nmax*2);
    captured.resize(nmax*2,false);
    for(n=0;n<nmax;n++){
        projector->queue(new GrayCodePattern(n,false));
        projector->queue(new GrayCodePattern(n,true));
    }
}

void BinaryCompiler::frameCaptured(
        cv::Mat frame,
        QCamera *cam,
        QProjector::Pattern *pattern)
{
    GrayCodePattern *gray =
            dynamic_cast<GrayCodePattern*>(pattern);
    uint idx;
    if(cam!=camera || !gray) return;
    idx = (gray->getBit()*2) + (gray->isInverted()?1:0);
    cv::cvtColor(frame,frames.at(idx),CV_BGR2GRAY);
    captured.at(idx) = true;
    std::cout << idx << "\n";
    for(idx=0;idx<captured.size();idx++){
        std::cout << (captured.at(idx)?"#":".");
    }
    std::cout << "\n";
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
                frames,20);
    emit binaryFrameCaptured(encoding);
    cv::Mat color = Triangulator::maphsv(encoding,
                                         1<<nmax);
    emit visualBinaryFrame(color);
    cv::imwrite("/home/ryan/nubinary.png",color);
}
