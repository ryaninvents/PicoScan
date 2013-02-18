#include "monotriangulator.h"

MonoTriangulator::MonoTriangulator(QObject *parent) :
    Triangulator(parent),
    camera(0)
{

}

void MonoTriangulator::setEncodingCamera(PixelEncodedCamera *cam)
{
    if(camera!=0)
        camera->disconnect(this);
    camera = cam;
    connect(camera,
            SIGNAL(frameCaptured(cv::Mat,QCamera::FrameType)),
            this,
            SLOT(frameReturned(cv::Mat,QCamera::FrameType)));
}

void MonoTriangulator::setCaptureCamera(QCamera *cam)
{
    if(camera==0) return;
    camera->setCapturingCamera(cam);
}

void MonoTriangulator::setProjector(QProjector *proj)
{
    projector = proj;
}

void MonoTriangulator::requestSheet()
{
    camera->requestFrame(QCamera::DOUBLE);
}

void MonoTriangulator::frameReturned(
        cv::Mat frame,
        QCamera::FrameType)
{
    uint x,y;
    cv::Vec3d P_up, P_fge, M_hat, D, pt;
    double ppx;
    Sheet sheet(cv::Size(frame.cols,frame.rows));

    D = projector->getPosition() - camera->getPosition();
    P_up = projector->getUpVector();

    for(x=0;x<frame.cols;x++){
        for(y=0;y<frame.rows;y++){
            ppx = frame.at<double>(y,x);
            if(ppx<0) continue;
            P_fge = projector->getPixelRay(ppx,0);
            M_hat = camera->getPixelRay(y,x);
            pt = sumTo(M_hat,P_up,P_fge,D);
            sheet.setPoint(x,y,pt);
        }
    }
    emit sheetComputed(sheet);
}
