#ifndef MODELVIEWWIDGET_H
#define MODELVIEWWIDGET_H

#include <QtOpenGL/QGLWidget>
#include "geom/sheet.h"

/// An OpenGL view for displaying the model.
class ModelViewWidget : public QGLWidget
{
    Q_OBJECT
public:
    /// Create the ModelViewWidget.
    explicit ModelViewWidget(QWidget *parent = 0);

    /// Conduct OpenGL initializing.
    void initializeGL();

    /// Paint the scene.
    void paintGL();

    /// Resize the panel.
    void resizeGL(int width, int height);

    /// Handle a mouse click.
    void mousePressEvent(QMouseEvent * ev);

    /// Handle a mouse move.
    void mouseMoveEvent(QMouseEvent * ev);

    /// Handle a mouse release.
    void mouseReleaseEvent(QMouseEvent *);

    /// Handle when the user scrolls the mouse wheel.
    void wheelEvent(QWheelEvent * ev);

    /// Change the displayed geometry.
    void setSheet(Sheet *s);


signals:

public slots:

    /// Zoom in by a predetermined factor.
    void zoomIn();

    /// Zoom out by a predetermined factor.
    void zoomOut();

    /// Zoom to the default level.
    void zoomFit();

private:
    /// model rotation about Y axis in 3D space; that is,
    /// in response to mouse X-movement
    GLdouble xRot;
    /// model rotation about X axis in 3D space; that is,
    /// in response to mouse Y-movement
    GLdouble yRot;
    /// zoom level
    GLdouble zoom;

    /// change in Y-rotation during the current drag operation
    GLdouble xPlus;

    /// change in X-rotation during the current drag operation
    GLdouble yPlus;

    /// mouse press position X
    int mX;
    /// mouse press position Y
    int mY;

    /// draw the "turntable" floor
    void drawFloor();

    /// draw the sheet
    void drawSheet();

    /// draw the placeholder cube
    void drawCube();

    /// the mesh
    Sheet *sheet;

};

#endif // MODELVIEWWIDGET_H
