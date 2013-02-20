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
    if(hasDependencies())
        emit aboutToAdvance();
    else
        processQueue();
}

bool QProjector::checkDependencies()
{
    if(!hasDependencies()){
        return true;
    }
    return listenersWaitingFor==0
            && listenersDenyingPermission==0;
}

bool QProjector::hasDependencies()
{
    return receivers(SIGNAL(aboutToAdvance()))>0;
}

void QProjector::processQueue()
{
    if(patternQueue.size()>0) {
        Pattern *pattern = patternQueue.at(patternQueue.size()-1);
        patternQueue.pop_back();
        if(pattern==0){
            processQueue();
            return;
        }
        projectPattern(pattern);
        listenersWaitingFor = 0;
        listenersDenyingPermission = 0;
        emit patternProjected(pattern,this);
    }
}

void QProjector::permissionToAdvance(bool canAdvance)
{
    listenersWaitingFor--;
    if(!canAdvance){
        listenersDenyingPermission++;
    }
    if(listenersWaitingFor==0
            && listenersDenyingPermission==0){
        processQueue();
    }
}
