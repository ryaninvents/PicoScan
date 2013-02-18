#include "projectordependent.h"

ProjectorDependent::ProjectorDependent(QObject *parent) :
    QObject(parent),
    satisfied(false)
{
}

void ProjectorDependent::setID(uint i)
{
    dependencyId = i;
}

bool ProjectorDependent::isSatisfied()
{
    return satisfied;
}

void ProjectorDependent::reset()
{
    satisfied = false;
}

void ProjectorDependent::satisfy()
{
    satisfied = true;
    emit permitRelease();
}

void ProjectorDependent::deregister()
{
    emit deregister(dependencyId);
}
