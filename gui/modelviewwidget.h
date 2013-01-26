#ifndef MODELVIEWWIDGET_H
#define MODELVIEWWIDGET_H

#include <QtOpenGL/QGLWidget>

class ModelViewWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit ModelViewWidget(QWidget *parent = 0);

    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);


    /*
      \todo mousePressEvent
      \todo mouseMoveEvent
      \todo mouseReleaseEvent
      */

    void mousePressEvent(QMouseEvent * ev);
    void mouseMoveEvent(QMouseEvent * ev);
    void mouseReleaseEvent(QMouseEvent *);
signals:

public slots:

private:
    // model rotation in 3D space
    GLdouble xRot, yRot, zoom;
    GLdouble xPlus, yPlus;

    // mouse press position
    int mX, mY;

};

#endif // MODELVIEWWIDGET_H
