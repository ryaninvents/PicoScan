#include "qprojector.h"

#include <QApplication>
#include <QDesktopWidget>

QProjector::QProjector(QObject *parent) :
    QOpticalDevice(parent),
    listenersWaitingFor(0),
    listenersDenyingPermission(0)
{
}

void QProjector::queue(QProjector::Pattern *p)
{
    emit debug(QString("Someone has queued a projector pattern."));
    emit debug(tr("Projector has %1 dependencies.")
               .arg(countDependencies()));
    patternQueue.push_back(p);
    emit debug(tr("Projector now has %1 patterns queued.")
               .arg(patternQueue.size()));
    requestAdvance();
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
    return countDependencies()>0;
}

uint QProjector::countDependencies()
{
    return receivers(SIGNAL(aboutToAdvance()));
}

void QProjector::processQueue()
{
    emit debug(tr("Projector processing queue of %1 patterns.")
               .arg(patternQueue.size()));
    if(patternQueue.size()>0) {
        Pattern *pattern = patternQueue.at(patternQueue.size()-1);
        patternQueue.pop_back();
        if(pattern==0){
            processQueue();
            return;
        }
        emit debug(tr("Projector projects, and announces pattern has been projected."));
        projectPattern(pattern);
        // handled by subclasses:
        // emit patternProjected
    }
    listenersWaitingFor = 0;
    listenersDenyingPermission = 0;
}

void QProjector::permissionToAdvance(bool canAdvance)
{
    emit debug(QString("Someone %1 the projector permission to advance.")
               .arg(canAdvance?"allows":"denies"));
    emit debug(QString("%1 listeners reporting, %2 denying.")
               .arg((int)listenersWaitingFor-1)
               .arg((int)listenersDenyingPermission+
                    (canAdvance?0:1)));
    if(listenersWaitingFor>0)
        listenersWaitingFor--;
    if(!canAdvance){
        listenersDenyingPermission++;
    }
    if(listenersWaitingFor==0
            && listenersDenyingPermission==0){
        processQueue();
    }
}

void QProjector::requestAdvance()
{
    emit debug(tr("Someone requests that the projector advance."));
    if(hasDependencies()){
        emit debug(QString("Projector announces intention to advance."));
        listenersWaitingFor = countDependencies();
        listenersDenyingPermission = 0;
        emit aboutToAdvance();
    }else{
        emit debug(QString("Projector directly processes pattern queue, as it has no listeners."));
        processQueue();
    }
}
