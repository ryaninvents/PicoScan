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

#define NUM_IMAGES 12

using namespace cv;
using namespace std;
using namespace fr;

int main(int, char**)
{
    VideoCapture cap0(0);
    VideoCapture cap1(1);
    if(!cap1.isOpened() || !cap0.isOpened())
        return -1;

    unsigned int i;

    cap1.set(CV_CAP_PROP_FRAME_WIDTH,640);
    cap1.set(CV_CAP_PROP_FRAME_HEIGHT,480);
    cap0.set(CV_CAP_PROP_FRAME_WIDTH,640);
    cap0.set(CV_CAP_PROP_FRAME_HEIGHT,480);
    Mat edges1, edges0;
    Size size(4,11);

    int counter = NUM_IMAGES;

    vector<Point2f> corners1,corners0;
    bool found1,found0;

    vector<Point3f> chess = fr::ChessGen::getBoard(size,10.16,true);
   /* for(unsigned int i=0;i<chess.size();i++){
        cout << chess[i].x << "," << chess[i].y << endl;
    }*/

    vector<vector<Point3f> > objectPoints;
    vector<vector<Point2f> > imagePoints1,imagePoints0;

    Mat camera0 = Mat::eye(3,3,CV_64F);
    Mat distortion0 = Mat::zeros(8, 1, CV_64F);
    Mat camera1 = Mat::eye(3,3,CV_64F);
    Mat distortion1 = Mat::zeros(8, 1, CV_64F);
    vector<Mat > rvecs0, rvecs1;
    vector<Mat > tvecs0, tvecs1;

    namedWindow("CAMERA 0",1);
    namedWindow("CAMERA 1",1);

    found0 = false;
    found1 = false;

    Mat frame0, frame1;

    for(;;)
    {
        cap1 >> frame1; // get a new frame from camera
        cvtColor(frame1, edges1, CV_BGR2GRAY);

        cap0 >> frame0;
        cvtColor(frame0,edges0,CV_BGR2GRAY);

        for(i=0;i<imagePoints1.size();i++){
            drawChessboardCorners(frame1,size,imagePoints1[i],true);
            drawChessboardCorners(frame0,size,imagePoints0[i],true);
        }

        imshow("CAMERA 0", frame0);
        imshow("CAMERA 1", frame1);

        if(waitKey(200)>=0){
            //found = findChessboardCorners(edges,size,corners);
            //*
            found0 = findCirclesGrid(edges0,size,corners0
                                    ,CALIB_CB_ASYMMETRIC_GRID
                                    );//*/
            found1 = findCirclesGrid(edges1,size,corners1
                                    ,CALIB_CB_ASYMMETRIC_GRID
                                    );//*/
            if(found0&&found1){
                objectPoints.push_back(chess);
                imagePoints0.push_back(corners0);
                imagePoints1.push_back(corners1);
                if(--counter<= 0)
                    break;
            }
        }
        //else waitKey(30);
        //if(waitKey(30)>1) break;
    }

    double rpe0 = calibrateCamera(objectPoints,imagePoints0,
                                 Size(edges0.size[0],edges0.size[1]),
                                 camera0,distortion0,rvecs0,tvecs0,
                                 CV_CALIB_FIX_ASPECT_RATIO|
                                 CV_CALIB_FIX_PRINCIPAL_POINT);
    double rpe1 = calibrateCamera(objectPoints,imagePoints1,
                                 Size(edges1.size[0],edges1.size[1]),
                                 camera1,distortion1,rvecs1,tvecs1,
                                 CV_CALIB_FIX_ASPECT_RATIO|
                                 CV_CALIB_FIX_PRINCIPAL_POINT);

    cout << " ==== Camera 0 ====\n";
    cout << camera0 << endl;
    cout << distortion0 << endl;

    cout << " ==== Camera 1 ====\n";
    cout << camera1 << endl;
    cout << distortion1 << endl;

    vector<Point3f> cornersReal;
    vector<Point2f> cornersImage;

    Mat relativeRot,relativeTrans;

    //cout << " ==== Computing positions... ===\n";

    for(unsigned int i=0;i<NUM_IMAGES;i++){
       // cout << tvecs0[i] << "\n";
        cornersReal.push_back(Point3f(tvecs0.at(i).at<double>(0),
                                      tvecs0.at(i).at<double>(1),
                                      tvecs0.at(i).at<double>(2)));
        cornersImage.push_back(imagePoints1[i][0]);
    }

    solvePnP(cornersReal,
             cornersImage,
             camera1,
             distortion1,
             relativeRot,
             relativeTrans);

    cout << " ==== Relative positions of cameras: ==== \n"
         << relativeTrans << "\n" << relativeRot << "\n";

    return 0;
}
