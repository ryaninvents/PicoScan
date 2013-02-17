#include "monotriangulator.h"

MonoTriangulator::MonoTriangulator(QObject *parent) :
    Triangulator(parent)
{
}

void MonoTriangulator::setCamera(PixelEncodedCamera *cam)
{
    camera = cam;

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
    cv::Vec3d P_up, P_fringe, M_hat, D, M;
    cv::Point3d pt;
    double ppx;
    Sheet sheet(cv::Size(frame.cols,frame.rows));

    D = projector->getPosition() - camera->getPosition();
    P_up = projector->getUpVector();

    for(x=0;x<frame.cols;x++){
        for(y=0;y<frame.rows;y++){
            ppx = frame.at<double>(y,x);
            if(ppx<0) continue;
            P_fringe = projector->getPixelRay(ppx,0);
            M_hat = camera->getPixelRay(y,x);
            M = sumTo(M_hat,P_up,P_fringe,D);
            pt = cv::Point3d(M[0],M[1],M[2]);

        }
    }
}
