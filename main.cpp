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
    VideoCapture cap1(1);
    if(!cap1.isOpened())
        return -1;

    unsigned int i;

    cap1.set(CV_CAP_PROP_FRAME_WIDTH,800);
    cap1.set(CV_CAP_PROP_FRAME_HEIGHT,600);
    Mat edges;
    Size size(4,11);

    int counter = 12;

    vector<Point2f> corners;
    bool found;

    vector<Point3f> chess = fr::ChessGen::getBoard(size,1,true);
   /* for(unsigned int i=0;i<chess.size();i++){
        cout << chess[i].x << "," << chess[i].y << endl;
    }*/

    vector<vector<Point3f> > objectPoints;
    vector<vector<Point2f> > imagePoints1,imagePoints0;

    Mat camera = Mat::eye(3,3,CV_64F);
    Mat distortion = Mat::zeros(8, 1, CV_64F);
    vector<Mat > rvecs;
    vector<Mat > tvecs;

    namedWindow("CAMERA 1",1);
    found = false;
    for(;;)
    {
        Mat frame;
        cap1 >> frame; // get a new frame from camera
        cvtColor(frame, edges, CV_BGR2GRAY);


        //frame.convertTo(edges,-1,0.2);

        for(i=0;i<imagePoints1.size();i++)
            drawChessboardCorners(frame,size,imagePoints1[i],true);

        imshow("CAMERA 1", frame);
        if(waitKey(200)>=0){
            //found = findChessboardCorners(edges,size,corners);
            //*
            found = findCirclesGrid(edges,size,corners
                                    ,CALIB_CB_ASYMMETRIC_GRID
                                    );//*/
            if(found){
                objectPoints.push_back(chess);
                imagePoints1.push_back(corners);
                if(--counter<= 0)
                    break;
            }
        }
        //else waitKey(30);
        //if(waitKey(30)>1) break;
    }

    double rpe = calibrateCamera(objectPoints,imagePoints1,Size(edges.size[0],edges.size[1]),
                                 camera,distortion,rvecs,tvecs,
                                 CV_CALIB_FIX_ASPECT_RATIO|
                                 CV_CALIB_FIX_PRINCIPAL_POINT);

    if(found) imwrite("/home/ryan/chessboard.png",edges);

    cout << camera << endl;
    //cout << rpe << endl;

    return 0;
}
