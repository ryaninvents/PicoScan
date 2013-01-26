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
#include "hardware/camera/camera.h"
#include "hardware/camera/opencvcamera.h"
#include "hardware/standards/calibrationstandard.h"
#include "hardware/standards/chessboardstandard.h"
#include "hardware/projector/projectionscreen.h"
/*!
\mainpage Scan Studio

Scan Studio allows users to quickly and easily scan 3D objects into the
industry-standard STL format.

\tableofcontents

\section License

\legalese
Copyright &copy; 2013, Ryan Muller and Chris Thomas.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
\list
\li Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
\li Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
\endlist

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

\endlegalese
*/


int main(int argc, char** argv)
{
//*
    QApplication a(argc, argv);
    //MainWindow w;
    ProjectionScreen w;
    w.showMaximized();
    w.projectOnDisplay(0);
    return a.exec();
//*/
    // desired number of images; eventually an argument?
    unsigned int n = 12;

    // the two cameras we'll be using
    Camera *cam1;
    Camera *cam2;

    // our calibration standard
    CalibrationStandard *standard =
            new ChessboardStandard(cv::Size(9,12),
                                   25.4);

    // keep track of how many images remain
    unsigned int counter = n;

    // list of snapshots
    std::vector<cv::Mat> images1;
    std::vector<cv::Mat> images2;

    // snapshot holders
    cv::Mat snap1, snap2;

    // initialize the cameras however you like
    // they will probably be arguments in the future
    cam1 = new OpenCVCamera(0);
    cam2 = new OpenCVCamera(1);

    // if we're having trouble, abort abort abort
    if(!cam1->isOpen() || !cam2->isOpen())
        return -1;

    cam1->setResolution(800,600);
    cam2->setResolution(800,600);

    cv::namedWindow("Camera 1",1);
    cv::namedWindow("Camera 2",1);

    for(;;){
        snap1 = cam1->getFrame();
        snap2 = cam2->getFrame();

        cv::imshow("Camera 1",snap1);
        cv::imshow("Camera 2",snap2);

        if(cv::waitKey(2)>1) break;

    }

    return 0;

    /*


    unsigned int i;
    Mat edges1, edges0;
    Size size(4,11);

    int counter = NUM_IMAGES;

    vector<Point2f> corners1,corners0;
    bool found1,found0;

    vector<Point3f> chess = fr::ChessGen::getBoard(size,10.16,true);

    vector<vector<Point3f> > objectPoints;
    vector<vector<Point2f> > imagePoints1,imagePoints0;

    Mat camera0mat = Mat::eye(3,3,CV_64F);
    Mat distortion0mat = Mat::zeros(8, 1, CV_64F);
    Mat camera1mat = Mat::eye(3,3,CV_64F);
    Mat distortion1mat = Mat::zeros(8, 1, CV_64F);
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
            //
            found0 = findCirclesGrid(edges0,size,corners0
                                    ,CALIB_CB_ASYMMETRIC_GRID
                                    );//
            found1 = findCirclesGrid(edges1,size,corners1
                                    ,CALIB_CB_ASYMMETRIC_GRID
                                    );//
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
                                 camera0mat,distortion0mat,rvecs0,tvecs0,
                                 CV_CALIB_FIX_ASPECT_RATIO|
                                 CV_CALIB_FIX_PRINCIPAL_POINT);
    double rpe1 = calibrateCamera(objectPoints,imagePoints1,
                                 Size(edges1.size[0],edges1.size[1]),
                                 camera1mat,distortion1mat,rvecs1,tvecs1,
                                 CV_CALIB_FIX_ASPECT_RATIO|
                                 CV_CALIB_FIX_PRINCIPAL_POINT);

    cout << " ==== Camera 0 ====\n";
    cout << camera0mat << endl;
    cout << distortion0mat << endl;

    cout << " ==== Camera 1 ====\n";
    cout << camera1mat << endl;
    cout << distortion1mat << endl;

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
             camera1mat,
             distortion1mat,
             relativeRot,
             relativeTrans);

    cout << " ==== Relative positions of cameras: ==== \n"
         << relativeTrans << "\n" << relativeRot << "\n";

    return 0;*/
}
