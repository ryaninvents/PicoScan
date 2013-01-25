#include "modelviewwidget.h"

#include <QtOpenGL>

ModelViewWidget::ModelViewWidget(QWidget *parent) :
    QGLWidget(parent)
{
}

void ModelViewWidget::initializeGL() {
    QColor black(36, 76, 120);
    qglClearColor(black);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void ModelViewWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
}

void ModelViewWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef QT_OPENGL_ES_1
    glOrthof(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
#else
    glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
#endif
    glMatrixMode(GL_MODELVIEW);
}
