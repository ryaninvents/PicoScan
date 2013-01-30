#include "projectionscreen.h"

#include <QApplication>
#include <QDesktopWidget>

ProjectionScreen::ProjectionScreen(QWidget *parent) :
    ImageViewWidget(parent)
{
    setWindowTitle("Projection screen");
    setCursor(QCursor(Qt::BlankCursor));

    QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::black);
    setAutoFillBackground(true);
    setPalette(pal);

    setContentsMargins(0,0,0,0);
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

void ProjectionScreen::displayPattern(ProjectionPattern *pattern)
{
    cv::Mat_<double> m = cv::Mat_<double>(geometry().width(),geometry().height());
    cv::Mat p = pattern->getPattern();
    double d;
    uint x,y;
    switch(pattern->getType()){
    case HORIZONTAL:
        for(x=0;x<pattern->getSize().width;x++){
            d = p.at<double>(x);
            for(y=0;y<geometry().height();y++){
                m.at<double>(x,y) = d;
            }
        }
    }
    displayImage(m,false);
}
