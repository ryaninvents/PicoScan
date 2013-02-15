#include "qprojector.h"

#include <QApplication>
#include <QDesktopWidget>

QProjector::QProjector(QObject *parent) :
    QOpticalDevice(parent)
{
}
