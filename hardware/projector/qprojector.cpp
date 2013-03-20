#include "qprojector.h"

#include <QApplication>
#include <QDesktopWidget>

#include "graycodepattern.h"

QProjector::QProjector(QObject *parent) :
    QOpticalDevice(parent),
    listenersWaitingFor(0),
    listenersDenyingPermission(0)
{
}

void QProjector::queue(QProjector::Pattern *p)
{
    emit debug(QString("Someone has queued a projector pattern."));
    emit debug(tr("Projector has %1 dependenc%2.")
               .arg(countDependencies())
               .arg(countDependencies()==1?"y":"ies"));
    patternQueue.push_back(p);
    emit debug(tr("Projector now has %1 pattern%2 queued.")
               .arg(patternQueue.size())
               .arg(patternQueue.size()==1?"":"s"));
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

QProjector::Pattern *QProjector::getCurrentPattern()
{
    return currentPattern;
}

void QProjector::processQueue()
{
    emit debug(tr("Projector processing queue of %1 pattern%2.")
               .arg(patternQueue.size())
               .arg(patternQueue.size()==1?"":"s"));
    if(patternQueue.size()>0) {
        Pattern *pattern = patternQueue.at(0);
        patternQueue.erase(patternQueue.begin());
        if(pattern==0){
            processQueue();
            return;
        }
        emit debug(tr("Projector projects, and announces pattern has been projected."));

        GrayCodePattern *gray =
                dynamic_cast<GrayCodePattern*>(pattern);

        if(gray){
            emit debug(tr("Projecting Gray code pattern,"
                      " bit %1, %2")
                   .arg(gray->getBit())
                   .arg(gray->isInverted()?
                            "inverted":
                            "not inverted"));
        }
        currentPattern = pattern;
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
    emit debug(QString("%1 listener%3 left to report, %2 denying.")
               .arg((int)listenersWaitingFor-1)
               .arg((int)listenersDenyingPermission+
                    (canAdvance?0:1))
               .arg((int)(listenersWaitingFor)==2?"":"s"));
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

void QProjector::Pattern::setID(int id)
{
    _id = id;
}

int QProjector::Pattern::getID()
{
    return _id;
}
