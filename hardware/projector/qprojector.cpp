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
    patternQueue.push_back(*p);
}

void QProjector::registerDependent(ProjectorDependent *dp)
{
    dp->setID(dependents.size());
    dependents.push_back(dp);
    connect(dp,SIGNAL(permitRelease()),
            this,SLOT(checkDependents()));
    connect(dp,SIGNAL(deregister(uint)),
            this,SLOT(deregisterDependent(uint)));
}

void QProjector::deregisterDependent(uint id)
{
    dependents.at(id)->disconnect();
    dependents.at(id) = 0;
}

void QProjector::processQueue()
{
    Pattern *pattern = patternQueue.pop_back();
    projectPattern(pattern);
    emit patternProjected(pattern);
}

void QProjector::checkDependents()
{
    uint i;
    for(i=0;i<dependents.size();i++){
        if(dependents.at(i)==0) continue;
        if(!(dependents.at(i)->isSatisfied())) return;
    }
    for(i=0;i<dependents.size();i++){
        if(dependents.at(i)==0) continue;
        dependents.at(i)->reset();
    }

}
