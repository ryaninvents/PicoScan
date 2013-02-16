#include "seconddisplayprojector.h"

#include <QApplication>
#include <QDesktopWidget>

SecondDisplayProjector::SecondDisplayProjector(QObject *parent) :
    QProjector(parent),
    w()
{
}

void SecondDisplayProjector::projectPattern(QProjector::Pattern *p, uint frameID)
{
    w.projectPattern(p);
    patternProjected(frameID);
}

void SecondDisplayProjector::Widget::projectOnDisplay(int n)
{
    if(n>=getDisplayCount()) return;
    QRect screenres = QApplication::desktop()->screenGeometry(n);
    move(QPoint(screenres.x(),screenres.y()));
    resize(screenres.width(),screenres.height());
    showFullScreen();
}

int SecondDisplayProjector::Widget::getDisplayCount()
{
    return QApplication::desktop()->screenCount();
}

void SecondDisplayProjector::Widget::projectPattern(QProjector::Pattern *p)
{
    QImage im = p->generatePattern(size().width(),size().height());
    displayImage(im);
}
