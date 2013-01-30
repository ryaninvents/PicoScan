#ifndef PROJECTIONSCREEN_H
#define PROJECTIONSCREEN_H

#include <QWidget>
#include "../../gui/imageviewwidget.h"
#include "../../hardware/projector/projectionpattern.h"


/**
  A widget that may be used to project full-screen images from a projector.
  */
class ProjectionScreen : public ImageViewWidget
{
    Q_OBJECT
public:
    explicit ProjectionScreen(QWidget *parent = 0);
    void projectOnDisplay(int n);
    int getDisplayCount();
    void displayPattern(ProjectionPattern *pattern);
    
signals:
    
public slots:
    
};

#endif // PROJECTIONSCREEN_H
