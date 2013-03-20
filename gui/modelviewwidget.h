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
    void mouseReleaseEvent(QMouseEvent * ev);

    /// Handle when the user scrolls the mouse wheel.
    void wheelEvent(QWheelEvent * ev);

    /// Change the displayed geometry.
    void setData(Sheet *data);


signals:

public slots:

    /// Zoom in by a predetermined factor.
    void zoomIn();

    /// Zoom out by a predetermined factor.
    void zoomOut();

    /// Zoom to the default level.
    void zoomFit();

    /// Center the model.
    void center();

private:
    /// model rotation about Y axis in 3D space; that is,
    /// in response to mouse X-movement
    GLdouble xRot;
    /// model rotation about X axis in 3D space; that is,
    /// in response to mouse Y-movement
    GLdouble yRot;
    /// zoom level
    GLdouble zoom;

    /// model translation in response to mouse
    cv::Vec3d trans;
    /// change in model translation during current drag op
    cv::Vec3d dTrans;

    /// change in Y-rotation during the current drag operation
    GLdouble xPlus;

    /// change in X-rotation during the current drag operation
    GLdouble yPlus;

    /// mouse press position X
    int mX;
    /// mouse press position Y
    int mY;

    /// mouse button
    uint mouseButton;

    /// draw the "turntable" floor
    void drawFloor();

    /// draw the sheet
    void drawCloud();

    /// draw the placeholder cube
    void drawCube();

    /// draw axes
    void drawAxes();

    /// draw a camera
    void drawCamera(float fWidth, float fDepth);
    /// draw a projector
    void drawProjector(float fDepth);

    /// get current rotation matrix for model view
    cv::Mat getCurrentModelRotation();

    /// the mesh
    std::vector<GLdouble> cloud;

    ///
    Sheet *sheet;

    void drawSheet();

};

#endif // MODELVIEWWIDGET_H
