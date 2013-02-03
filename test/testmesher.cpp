#include "testmesher.h"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "../hardware/opticaldevice.h"
#include "../geom/pointcloud.h"
#include "../geom/triangulator.h"

#define FILE_PATTERN "/home/ryan/Pictures/Round 2/picture_%d.png"
#define QUALITY_THRESHOLD 50

/**
  Gray code functions adapted from http://en.wikipedia.org/wiki/Gray_code
  under the Creative Commons CC-BY-SA 3.0 license
  */
/*
        The purpose of this function is to convert an unsigned
        binary number to reflected binary Gray code.

        The operator >> is shift right. The operator ^ is exclusive or.
*/
int binaryToGray(int num)
{
        return (num >> 1) ^ num;
}

/*
        The purpose of this function is to convert a reflected binary
        Gray code number to a binary number.
*/
int grayToBinary(int num)
{
    unsigned int numBits = 8 * sizeof(num);
    unsigned int shift;
    for (shift = 1; shift < numBits; shift = 2 * shift)
    {
        num = num ^ (num >> shift);
    }
    return num;
}


void TestMesher::unit()
{
    cv::Vec3d M_hat = cv::Vec3d(1,-1,0);
    cv::Vec3d P_up = cv::Vec3d(0,-1,0);
    cv::Vec3d P_fwd = cv::Vec3d(0,0,1);
    cv::Vec3d D = cv::Vec3d(1,0,0);
    cv::Vec3d M = Triangulator::sumTo(M_hat,P_up,P_fwd,D);

    std::cout << "Triangulator::sumTo\t" << cv::Mat(M) << "\n";

    OpticalDevice *projector = new OpticalDevice();
    projector->setResolution(3,3);
    projector->setFocalLength(1.0);
    projector->setPrincipalPoint(1,1);

    M = projector->getPixelRay(0,0);
    std::cout << "getPixelRay(0,0)\t" << cv::Mat(M) << "\n";

    M = projector->getUpVector();
    std::cout << "getUpVector\t" << cv::Mat(M) << "\n";

    M = projector->getFwdVector();
    std::cout << "getFwdVector\t" << cv::Mat(M) << "\n";

    M = projector->getPixelRay(1,1);
    std::cout << "getPixelRay(1,1)\t" << cv::Mat(M) << "\n";

}

void TestMesher::testMesh(){

    // character buffer for generating file names
    char filename[50];

    // current image index
    unsigned int idx = 1;

    // bits necessary to uniquely identify each pixel
    unsigned int nmax;

    // current bit (loop counter)
    unsigned int n;

    // frame holders (within loop)
    cv::Mat img, inv;

    // pixel loop indices
    unsigned int x, y;

    // quality mask; discard unreliable points
    cv::Mat qmask;

    // Gray-encoded image
    cv::Mat encoded;

    // decoded image
    cv::Mat decoded;

    // the resultant point cloud
    PointCloud cloud;

    // aaaand hopefully I don't need to explain these to you:
    OpticalDevice *camera = new OpticalDevice();
    OpticalDevice *projector = new OpticalDevice();

    // set up the camera
    camera->setResolution(640, 480);
    camera->setFocalLength(800);
    camera->setPrincipalPoint(320, 240);
    camera->setPosition(cv::Vec3d());

    // set up the projector
    projector->setResolution(1024,1);
    projector->setFocalLength(5000);
    projector->setPrincipalPoint(512,0);
    projector->setPosition(cv::Vec3d(0.3,0.2,0.0));
    projector->setOrientation(cv::Vec3d(0.0,-0.3,0.0));

    // determine the number of bits we need to project
    nmax = (unsigned int) ceil(log(projector->getResolutionU())/log(2));

    // initialize the quality mask
    qmask = cv::Mat::ones(camera->getResolution(),CV_32S);


    // construct the qmask filename
    sprintf(filename, FILE_PATTERN, 22);

    // load the frame
    img = cv::imread(filename,CV_LOAD_IMAGE_GRAYSCALE);
    img.convertTo(img,CV_32S);

    // construct the next filename
    sprintf(filename, FILE_PATTERN, 21);

    // load the frame
    inv = cv::imread(filename,CV_LOAD_IMAGE_GRAYSCALE);
    inv.convertTo(inv,CV_32S);

    // take the difference between the images
    qmask = img - inv;

    // initialize the encoded/decoded matrices
    encoded = cv::Mat::zeros(camera->getResolution(),CV_32S);
    decoded = cv::Mat::zeros(camera->getResolution(),CV_32S);

    // loop through the images
    for(n=0;n<nmax;n++){

        // construct the next filename
        sprintf(filename, FILE_PATTERN, idx++);

        // load the frame
        img = cv::imread(filename,CV_LOAD_IMAGE_GRAYSCALE);
        img.convertTo(img,CV_32S);

        // construct the next filename
        sprintf(filename, FILE_PATTERN, idx++);

        // load the frame
        inv = cv::imread(filename,CV_LOAD_IMAGE_GRAYSCALE);
        inv.convertTo(inv,CV_32S);

        // take the difference between the images
        img = img - inv;

        // loop through every pixel
        for(x=0; x<img.cols; x++){
            for(y=0; y<img.rows; y++){
                if(abs(img.at<int>(y,x))<QUALITY_THRESHOLD){
                    qmask.at<unsigned char>(y,x) = 0;
                }
                if(img.at<int>(y,x)>0){
                    encoded.at<int>(y,x) += 1<<n;
                }
            }
        }
    }

    // loop through every pixel
    for(x=0; x<img.cols; x++){
        for(y=0; y<img.rows; y++){
            if(abs(qmask.at<int>(y,x)) > QUALITY_THRESHOLD){
                decoded.at<int>(y,x) = grayToBinary(encoded.at<int>(y,x));
            }else{
                decoded.at<int>(y,x) = -1;
            }
        }
    }

    cv::imwrite("/home/ryan/Documents/decoded.png",decoded);

    int px;
    cv::Vec3d P_up, P_fwd, M_hat, D, M;
    cv::Point3d pt;

    D = projector->getPosition() - camera->getPosition();
    //std::cout << projector->getOrientation() << "\n";
    P_up = projector->getUpVector();
    std::cout << "Principal: ";
    std::cout << projector->getPixelRay(projector->getPrincipalU(),
                                   projector->getPrincipalV());
    std::cout << "\ncorresponding to (" << projector->getPrincipalU();
    std::cout << "," << projector->getPrincipalV() << ")\n";

    std::ofstream log;
    log.open("/home/ryan/Documents/geom.csv");
    log << "u,v,u_p,P_up.x,P_up.y,P_up.z,"
           "P_fwd.x,P_fwd.y,P_fwd.z,"
           "M_hat.x,M_hat.y,M_hat.z,"
           "D.x,D.y,D.z,M.x,M.y,M.z\n";

    for(x=0; x<img.cols; x++){
        for(y=0; y<img.rows; y++){
            px = decoded.at<int>(y,x);
            if(px<0) continue;
            P_fwd = projector->getPixelRay(px,0);
            M_hat = camera->getPixelRay(x,y);
            M = Triangulator::sumTo(M_hat,P_up,P_fwd,D);
            pt = cv::Point3d(M[0],M[1],M[2]);
            cloud.add(pt);
            log << x << ',' << y << ',' << px << ',' <<
                   P_up[0] << ',' << P_up[1] << ',' <<
                   P_up[2] << ',' << P_fwd[0] << ',' <<
                   P_fwd[1] << ',' << P_fwd[2] << ',' <<
                   M_hat[0] << ',' << M_hat[1] << ',' <<
                   M_hat[2] << ',' << D[0] << ',' <<
                   D[1] << ',' << D[2] << ',' << M[0] <<
                   ',' << M[1] << ',' << M[2] << '\n';

        }
    }

    log.close();

    cloud.meshlab("/home/ryan/Documents/mqp-data/head.obj");

}
