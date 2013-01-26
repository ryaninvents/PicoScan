#ifndef PROJECTIONSCREEN_H
#define PROJECTIONSCREEN_H

#include <QWidget>


/**
  A widget that may be used to project full-screen images from a projector.
  */
class ProjectionScreen : public QWidget
{
    Q_OBJECT
public:
    explicit ProjectionScreen(QWidget *parent = 0);
    void projectOnDisplay(int n);
    int getDisplayCount();
protected:
    void paintEvent(QPaintEvent *);
    
signals:
    
public slots:
    
};

#endif // PROJECTIONSCREEN_H
