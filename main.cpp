#include <QtGui/QApplication>
#include "mainwindow.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <stdio.h>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,480);
    Mat edges;
    Size size(7,7);
    //Size size(9,17);
    vector<Point2f> corners;
    bool found;

    namedWindow("edges",1);
    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        cvtColor(frame, edges, CV_BGR2GRAY);
        found = findChessboardCorners(edges,size,corners);
        /*
        found = findCirclesGrid(edges,size,corners
                                ,CALIB_CB_ASYMMETRIC_GRID
                                );//*/
        if(found) frame.convertTo(edges,-1,0.2);
        //else displayOverlay("edges","no chessboard found",0);
        drawChessboardCorners(edges,size,corners,found);
        //GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
        //Canny(edges, edges, 0, 60, 3);
        imshow("edges", edges);
        if(found){if(waitKey(30) >= 0) break;}
        else{if(waitKey(300) >= 0) break;}
    }
    if(found) imwrite("/home/ryan/chessboard.png",edges);

    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
