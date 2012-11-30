#include <QtGui/QApplication>
#include "mainwindow.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <stdio.h>
#include <iostream>
#include <vector>

#include "geom/chessgen.h"

using namespace cv;
using namespace std;
using namespace fr;

int main(int, char**)
{
    VideoCapture cap(1); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    cap.set(CV_CAP_PROP_FRAME_WIDTH,800);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,600);
    Mat edges;
    Size size(9,17);

    int counter = 10;

    vector<Point2f> corners;
    bool found;

    vector<Point3f> chess = fr::ChessGen::getBoard(size,1,true);
   /* for(unsigned int i=0;i<chess.size();i++){
        cout << chess[i].x << "," << chess[i].y << endl;
    }*/

    vector<vector<Point3f> > objectPoints;
    vector<vector<Point2f> > imagePoints;

    Mat camera = Mat::eye(3,3,CV_64F);
    Mat distortion = Mat::zeros(8, 1, CV_64F);
    vector<Mat > rvecs;
    vector<Mat > tvecs;

    namedWindow("edges",1);
    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        cvtColor(frame, edges, CV_BGR2GRAY);

        found = findCirclesGrid(edges,size,corners
                                ,CALIB_CB_ASYMMETRIC_GRID
                                );//*/
        if(found) frame.convertTo(edges,-1,0.2);

        drawChessboardCorners(edges,size,corners,found);

        imshow("edges", edges);
        if(found){
            if(waitKey(200)>=0){
            //waitKey(300);
                objectPoints.push_back(chess);
                imagePoints.push_back(corners);
                if(--counter<= 0)
                    break;
            }
        }
        else waitKey(30);
    }

    calibrateCamera(objectPoints,imagePoints,Size(800,600),camera,distortion,rvecs,tvecs,0);

    if(found) imwrite("/home/ryan/chessboard.png",edges);

    cout << camera << endl;

    return 0;
}
