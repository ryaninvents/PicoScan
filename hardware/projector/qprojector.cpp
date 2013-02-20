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
    return dependencies.empty();
}

void QProjector::processQueue()
{
    if(checkDependencies()) {
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

void QProjector::waitFor(QObject *holdup)
{
    uint i;
    for(i=0;i<dependencies.size();i++){
        if(dependencies.at(i)==holdup){
            return;
        }
    }
    dependencies.push_back(holdup);
}

void QProjector::stopWaitingFor(QObject *holdup)
{
    int i,idx=-1;
    for(i=0;i<dependencies.size();i++){
        if(dependencies.at(i)==holdup){
            idx = i;
        }
    }
    if(idx<0) return;
    dependencies.erase(dependencies.begin()+idx);
}
