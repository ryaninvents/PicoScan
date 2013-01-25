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
signals:

public slots:

private:
    // model rotation in 3D space
    double xRot, yRot, zRot;

};

#endif // MODELVIEWWIDGET_H
