#include "projectionscreen.h"

#include <QApplication>
#include <QDesktopWidget>

ProjectionScreen::ProjectionScreen(QWidget *parent) :
    QWidget(parent)
{
    setWindowTitle("Projection screen");
    setCursor(QCursor(Qt::BlankCursor));

    QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::black);
    setAutoFillBackground(true);
    setPalette(pal);
}

void ProjectionScreen::projectOnDisplay(int n)
{
    if(n>=getDisplayCount()) return;
    QRect screenres = QApplication::desktop()->screenGeometry(n);
    move(QPoint(screenres.x(),screenres.y()));
    resize(screenres.width(),screenres.height());
    showFullScreen();
}

int ProjectionScreen::getDisplayCount()
{
    return QApplication::desktop()->screenCount();
}

void ProjectionScreen::paintEvent(QPaintEvent *)
{

}
