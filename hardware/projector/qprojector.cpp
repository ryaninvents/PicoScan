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
        if(patternQueue.at(i)->getPatternID()==p->getPatternID()){
            return;
        }
    }
    patternQueue.push_back(p);
    processQueue();
}

void QProjector::registerDependency(ProjectorDependent *dp)
{
    dp->setID(dependents.size());
    dependents.push_back(dp);
    connect(dp,SIGNAL(permitRelease()),
            this,SLOT(processQueue()));
    connect(dp,SIGNAL(deregister(uint)),
            this,SLOT(deregisterDependency(uint)));
}

void QProjector::deregisterDependency(uint id)
{
    dependents.at(id)->disconnect();
    dependents.at(id) = 0;
}

void QProjector::deregisterDependencies()
{
    uint i;
    for(i=0;i<dependents.size();i++){
        deregisterDependency(i);
    }
    dependents.clear();
}

void QProjector::processQueue()
{
    if(checkDependencies()) {
        Pattern *pattern = patternQueue.at(patternQueue.size()-1);
        patternQueue.pop_back();
        projectPattern(pattern);
        emit patternProjected(pattern);
    }
}

bool QProjector::checkDependencies()
{
    uint i;
    for(i=0;i<dependents.size();i++){
        if(dependents.at(i)==0) continue;
        if(!(dependents.at(i)->isSatisfied())) return false;
    }
    for(i=0;i<dependents.size();i++){
        if(dependents.at(i)==0) continue;
        dependents.at(i)->reset();
    }
    return true;

}
