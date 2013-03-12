#include "modelviewwidget.h"

#include <QtOpenGL>
#include <stdio.h>
#define MAX_ZOOM -2.0
#define MIN_ZOOM -90.0
#define DEFAULT_ZOOM -8.0

ModelViewWidget::ModelViewWidget(QWidget *parent) :
    QGLWidget(parent),
    cloud()
{
}

void ModelViewWidget::initializeGL() {
    QColor bgBlue(200,200,200);
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
    xRot = -18.0;
    yRot = 12.0;
}

void ModelViewWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0,0.0,zoom);
    glRotatef(yRot+yPlus, 1.0, 0.0, 0.0);
    glRotatef(xRot+xPlus, 0.0, 1.0, 0.0);
    drawCloud();
//    drawFloor();
    drawAxes();
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
    if(ev->buttons()==Qt::LeftButton){
        int x = ev->x() - mX;
        int y = ev->y() - mY;
        xPlus = (GLdouble)x/10;
        yPlus = (GLdouble)y/10;
        if(yRot+yPlus > 90) yPlus = 90 - yRot;
        if(yRot+yPlus < -90) yPlus = -90 - yRot;
        updateGL();
    }
}

void ModelViewWidget::mouseReleaseEvent(QMouseEvent *)
{
    xRot+=xPlus;
    yRot+=yPlus;
    xPlus = 0;
    yPlus = 0;
    releaseMouse();
}

void ModelViewWidget::wheelEvent(QWheelEvent *ev)
{
    zoom += (GLdouble)ev->delta()/200.0;
    if(zoom>MAX_ZOOM) zoom = MAX_ZOOM;
    if(zoom<MIN_ZOOM) zoom = MIN_ZOOM;
    updateGL();
}

void ModelViewWidget::setData(std::vector<cv::Vec3d> pts)
{
    uint i;
    cloud.clear();
    for(i=0;i<pts.size();i++){
        cloud.push_back((GLdouble)pts.at(i)[0]);
        cloud.push_back((GLdouble)pts.at(i)[1]);
        cloud.push_back((GLdouble)pts.at(i)[2]);
    }
    printf("added %d points to model view\n",
           cloud.size()/3);
    updateGL();
}

void ModelViewWidget::zoomIn()
{
    zoom*=0.9;
    if(zoom>MAX_ZOOM) zoom = MAX_ZOOM;
    updateGL();
}

void ModelViewWidget::zoomOut()
{
    zoom/=0.9;
    if(zoom<MIN_ZOOM) zoom = MIN_ZOOM;
    updateGL();
}

void ModelViewWidget::zoomFit()
{
    zoom = DEFAULT_ZOOM;
    updateGL();
}

void ModelViewWidget::drawFloor()
{
    glPushMatrix();
    glTranslatef(0.0,-1.0,0.0);
    glColor3f(0.5,0.5,0.5);
    double r, theta;
    double rMax = 5;
    double rMin = 0.5;
    double rStep = 0.5;
    double thetaStep = 3.14159/16;
    GLdouble x,z;
    for(r=rMax;r>=rMin;r-=rStep){
        glBegin(GL_LINE_LOOP);
        for(theta=0;theta<3.14159*2;theta+=thetaStep){
            x=cos(theta)*r;
            z=sin(theta)*r;
            glVertex3f(x,0.0,z);
        }
        glEnd();
    }
    glBegin(GL_LINES);{
        for(theta=0;theta<3.14159*2;theta+=thetaStep){
            x=cos(theta)*rMax;
            z=sin(theta)*rMax;
            glVertex3f(x,0.0,z);
            x=cos(theta)*rMin;
            z=sin(theta)*rMin;
            glVertex3f(x,0.0,z);
        }
    } glEnd();
    glPopMatrix();
}

#define MODEL_SCALE 5

void ModelViewWidget::drawCloud()
{
    if(cloud.size()<10){
//        drawCube();
        return;
    }
    uint i;
    glBegin(GL_POINTS);{
//        glEnableClientState(GL_VERTEX_ARRAY);
        glColor3f(0,0,0);
        glDisable(GL_LIGHT0);
        glPointSize(5.0);
//        glVertexPointer((GLint)cloud.size(),
//                        GL_DOUBLE,
//                        (GLsizei)0,
//                        &(cloud[0]));
//        glDrawArrays(GL_POINTS,0,cloud.size()/3);
//        glDisableClientState(GL_VERTEX_ARRAY);
        for(i=0;i<cloud.size();i+=3){
            glVertex3f(cloud.at(i)*MODEL_SCALE,
                       cloud.at(i+1)*MODEL_SCALE,
                       cloud.at(i+2)*MODEL_SCALE);
        }
    } glEnd();
}

void ModelViewWidget::drawCube()
{

    GLfloat x, y, z;
    GLfloat LO=-.5, HI=.5, gap = 1;
    glBegin(GL_LINES);{
        glColor3f(0,0,0);
        for(x=LO;x<=HI;x+=gap){
            for(y=LO;y<=HI;y+=gap){
                for(z=LO;z<=HI;z+=gap){
                    glVertex3f(x,y,z);
                }
            }
        }
        for(z=LO;z<=HI;z+=gap){
            for(x=LO;x<=HI;x+=gap){
                for(y=LO;y<=HI;y+=gap){
                    glVertex3f(x,y,z);
                }
            }
        }
        for(y=LO;y<=HI;y+=gap){
            for(z=LO;z<=HI;z+=gap){
                for(x=LO;x<=HI;x+=gap){
                    glVertex3f(x,y,z);
                }
            }
        }

    } glEnd();
}

void ModelViewWidget::drawAxes()
{
    glBegin(GL_LINES);{
        glColor3f(0.5,0.0,0.0);
        glVertex3f(0.0,0.0,0.0);
        glVertex3f(1.0,0.0,0.0);
        glColor3f(0.0,0.5,0.0);
        glVertex3f(0.0,0.0,0.0);
        glVertex3f(0.0,1.0,0.0);
        glColor3f(0.0,0.0,0.5);
        glVertex3f(0.0,0.0,0.0);
        glVertex3f(0.0,0.0,1.0);
    } glEnd();
}

