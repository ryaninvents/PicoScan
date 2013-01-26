#include "modelviewwidget.h"

#include <QtOpenGL>

ModelViewWidget::ModelViewWidget(QWidget *parent) :
    QGLWidget(parent)
{
}

void ModelViewWidget::initializeGL() {
    QColor bgBlue(36, 76, 120);
    qglClearColor(bgBlue);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    //srand(5);
    zoom = -8.0;
}

void ModelViewWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0,0.0,zoom);
    glRotatef(xRot+xPlus, 0.0, 1.0, 0.0);
    glRotatef(yRot+yPlus, 1.0, 0.0, 0.0);

    GLfloat x, y, z;
    GLfloat LO=-.5, HI=.5, gap = 0.0625;

    glBegin(GL_POINTS);
    glColor3f(1.0,1.0,1.0);
    for(x=LO;x<=HI;x+=gap){
        for(y=LO;y<=HI;y+=gap){
            for(z=LO;z<=HI;z+=gap){
                glVertex3f(x,y,z);
            }
        }
    }
    glEnd();
    glFlush();
}

void ModelViewWidget::resizeGL(int width, int height)
{
    //int side = qMin(width, height);
    GLdouble aspect = (GLdouble)width/height;
    //glViewport((width - side) / 2, (height - side) / 2, side, side);
    glViewport(0,0,width,height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef QT_OPENGL_ES_1
    //glOrthof(-0.5, +0.5, -0.5, +0.5, 1.0, 15.0);
#else
    //glOrtho(-0.5, +0.5, -0.5, +0.5, 1.0, 15.0);
    //gluPerspective(60.0,(GLdouble)((double)height/width),1.0,15);
    glFrustum(-0.5,+0.5,-0.5/aspect,+0.5/aspect,1.0,150.0);
#endif
    glMatrixMode(GL_MODELVIEW);
}

void ModelViewWidget::mousePressEvent(QMouseEvent *ev)
{
    mX = ev->x();
    mY = ev->y();
    grabMouse(QCursor(Qt::ClosedHandCursor));
}

void ModelViewWidget::mouseMoveEvent(QMouseEvent *ev)
{
    int x = ev->x() - mX;
    int y = ev->y() - mY;
    xPlus = (GLdouble)x/10;
    yPlus = (GLdouble)y/10;
    updateGL();
}

void ModelViewWidget::mouseReleaseEvent(QMouseEvent *)
{
    xRot+=xPlus;
    yRot+=yPlus;
    releaseMouse();
}

