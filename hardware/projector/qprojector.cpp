#include "qprojector.h"

#include <QApplication>
#include <QDesktopWidget>

QProjector::QProjector(QObject *parent) :
    QOpticalDevice(parent)
{
}

void QProjector::queue(QProjector::Pattern *p)
{
    uint i;
    for(i=0;i<patternQueue.size();i++){
        if(patternQueue.at(i)==0) continue;
        if(patternQueue.at(i)->getPatternID()==p->getPatternID()){
            return;
        }
    }
    patternQueue.push_back(p);
    processQueue();
}

bool QProjector::checkDependencies()
{
    return receivers(SIGNAL(hold()))==0;
}

void QProjector::processQueue()
{
    if(checkDependencies()
            && patternQueue.size()>0) {
        Pattern *pattern = patternQueue.at(patternQueue.size()-1);
        patternQueue.pop_back();
        if(pattern==0){
            processQueue();
            return;
        }
        projectPattern(pattern);
        emit patternProjected(pattern,this);
    }
}

void QProjector::waitFor(QObject *obj, const char *slot)
{
    connect(this,
            SIGNAL(hold()),
            obj,
            slot);
}

void QProjector::stopWaitingFor(QObject *obj)
{
    disconnect(SIGNAL(hold()),obj);
}

void QProjector::disconnectNotify(const char *signal)
{
    if(signal==SIGNAL(hold())){
        processQueue();
    }
}
