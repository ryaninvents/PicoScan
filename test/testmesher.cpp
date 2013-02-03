#include "testmesher.h"

#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "../hardware/opticaldevice.h"
#include "../geom/pointcloud.h"

#define FILE_PATTERN "/home/ryan/Pictures/Round 2/picture_%d.png"
#define QUALITY_THRESHOLD 20

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
    camera->setFocalLength(50/8);
    camera->setPrincipalPoint(320, 240);

    // set up the projector
    projector->setResolution(1024,768);
    projector->setFocalLength(3*1024/16);
    projector->setPrincipalPoint(512,384);
    projector->setPosition(cv::Vec3d(0.3,0.1,0.0));
    projector->setOrientation(cv::Vec3d(0.0,0.297,0.0));

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
                decoded.at<int>(y,x) = grayToBinary(encoded.at<int>(y,x))%256;
            }
        }
    }

    cv::imwrite("/home/ryan/Documents/decoded.png",decoded);
}
