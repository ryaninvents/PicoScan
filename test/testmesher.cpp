#include "testmesher.h"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "../hardware/opticaldevice.h"
#include "../geom/pointcloud.h"
#include "../geom/triangulator.h"
#include "../geom/sheet.h"

#include <QString>

#define FILE_PATTERN "/home/ryan/Pictures/Round 2/picture_%d.png"
#define QUALITY_THRESHOLD 20

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


cv::Mat getAvgImg(QString fnmPattern, int avg){
    uint i;
    cv::Mat out, m;
    out = cv::imread(fnmPattern.arg(0)
                     .toLocal8Bit().data(),
                     CV_LOAD_IMAGE_GRAYSCALE);
    out.convertTo(out,CV_64F);
    if(avg<2) return out;
    for(i=1;i<avg;i++){
        m = cv::imread(fnmPattern.arg(i)
                       .toLocal8Bit().data(),
                       CV_LOAD_IMAGE_GRAYSCALE);
        m.convertTo(m,CV_64F);
        out = out + m;
    }
    out = out / avg;
    return out;
}

void TestMesher::testMesh(char *configFilename){

    std::cout << "loading from " << configFilename << std::endl;

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

    // config file
    FILE * config;

    config = fopen(configFilename,"r");

    int camResX, camResY;
    int camFocal;

    int projectorRes;
    int projectorFocal;
    double pPosX, pPosY, pPosZ;
    double pOriX, pOriY, pOriZ;
    char imPath[128];
    int ignoreBits;
    int avg;

    QString qFnm;
    FileSource source;

    //======= start reading config file =======//

    /*
    if(!fscanf(config,"#CAMERA")){
        printf("Syntax error: missing \"#CAMERA\"\n");
        return;
    }*/
    if(!fscanf(config,"resolution %dx%d\n",&camResX,&camResY)){
        printf("Syntax error: missing \"resolution ___x___\"\n");
        return;
    }
    if(!fscanf(config,"focal %d\n",&camFocal)){
        printf("Syntax error: missing \"focal ___ (camera)\"\n");
        return;
    }/*
    if(!fscanf(config,"#PROJECTOR")){
        printf("Syntax error: missing \"#PROJECTOR\"\n");
        return;
    }*/
    if(!fscanf(config,"resolution %d\n",&projectorRes)){
        printf("Syntax error: missing \"resolution ___\"\n");
        return;
    }
    if(!fscanf(config,"focal %d\n",&projectorFocal)){
        printf("Syntax error: missing \"focal ___ (projector)\"\n");
        return;
    }
    if(!fscanf(config,"posn %lf %lf %lf\n",&pPosX,&pPosY,&pPosZ)){
        printf("Syntax error: missing \"posn __ __ __\"\n");
        return;
    }
    if(!fscanf(config,"orin %lf %lf %lf\n",&pOriX,&pOriY,&pOriZ)){
        printf("Syntax error: missing \"orin __ __ __\"\n");
        return;
    }/*
    if(!fscanf(config,"#INPUT")){
        printf("Syntax error: missing \"#INPUT\"\n");
        return;
    }*/

    if(!fscanf(config,"ignore %d\n",&ignoreBits)){
        printf("Warning: missing \"ignore __\"\n");
        ignoreBits = 0;
    }
    if(!fscanf(config,"avg %d\n",&avg)){
        printf("Warning: missing \"avg __\"\n");
        avg = 1;
    }
    if(!fscanf(config,"images %[^\r\n]s\n",imPath)){
        printf("Syntax error: missing \"images __________\"\n");
        return;
    }

    qFnm = QString(imPath);
    source.setDirectory(imPath);
    source.setAveraging(avg);

    //======== end reading config file ========//
    printf("Camera resolution %dx%d\n",camResX,camResY);
    printf("Camera focal length %d\n", camFocal);
    printf("Projector resolution %d\n",projectorRes);
    printf("Projector focal length %d\n",projectorFocal);
    printf("Projector position %f %f %f\n",pPosX,pPosY,pPosZ);
    printf("Projector orientation %f %f %f\n",pOriX,pOriY,pOriZ);
    printf("Image path \"%s\"\n",imPath);
    printf("Ignore %d bits\n",ignoreBits);
    printf("Averaging %d images together\n",avg);


    // set up the camera
    camera->setResolution(camResX, camResY);
    camera->setFocalLength(camFocal);
    camera->setPrincipalPoint(camResX/2, camResY/2);
    camera->setPosition(cv::Vec3d());

    // set up the projector
    //projectorRes = projectorRes >> ignoreBits;
    //projectorFocal = projectorFocal >> ignoreBits;
    projector->setResolution(projectorRes,1);
    projector->setFocalLength(projectorFocal);
    projector->setPrincipalPoint(projectorRes/2,0);
    projector->setPosition(cv::Vec3d(pPosX,pPosY,pPosZ));
    projector->setOrientation(cv::Vec3d(pOriX,pOriY,pOriZ));

    // determine the number of bits we need to project
    nmax = (unsigned int) ceil(log(projector->getResolutionU())/log(2));

    // load the qmask frame
    //img = cv::imread(filename,CV_LOAD_IMAGE_GRAYSCALE);
    /*img = getAvgImg(qFnm.arg(10)
                    .arg("neg"),avg);*/
    img = source.getAveragedGrayCodeImage(10,false);
    img.convertTo(img,CV_32S);

    // load the frame
    inv = source.getAveragedGrayCodeImage(10,true);
    inv.convertTo(inv,CV_32S);

    // initialize the encoded/decoded matrices
    encoded = cv::Mat::zeros(camera->getResolution(),CV_32S);
    decoded = cv::Mat::zeros(camera->getResolution(),CV_32S);

    nmax = 10;

    std::vector<cv::Mat> bitImages;

    // loop through the images
    for(n=0;n<nmax;n++){
        img = source.getAveragedGrayCodeImage(n,false);
        img.convertTo(img,CV_32S);
        bitImages.push_back(img);

        inv = source.getAveragedGrayCodeImage(n,true);
        inv.convertTo(inv,CV_32S);
        bitImages.push_back(inv);
    }

    decoded = Triangulator::computeBinary(bitImages,20);

    writeToHueFile("/home/ryan/Documents/mqp-data/new-output.png",
                   decoded,
                   640);

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

    uint decimation=3;

    Sheet sheet(cv::Size(img.cols,img.rows));

    for(x=0; x<img.cols-decimation; x+=decimation){
        for(y=0; y<img.rows-decimation; y+=decimation){
            px = decoded.at<int>(y,x);
            if(px<0) continue;
            P_fwd = projector->getPixelRay(px,0);
            M_hat = camera->getPixelRay(x,y);
            M = Triangulator::sumTo(M_hat,P_up,P_fwd,D);
            pt = cv::Point3d(M[0],M[1],M[2]);
            cloud.add(pt);

            sheet.setPoint(x/decimation,y/decimation,M);
            if(     (decoded.at<int>(y+decimation,x)>0)&&
                    (decoded.at<int>(y,x+decimation)>0)&&
                    (decoded.at<int>(y+decimation,x+decimation)>0)){
                sheet.enableQuad(x/decimation,y/decimation);
            }

        }
    }

    sheet.saveSTL("/home/ryan/Documents/mqp-data/hemi.stl");

    cloud.meshlab("/home/ryan/Documents/mqp-data/hemi.obj");

}


void TestMesher::testPhase(char *configFilename){

    // character buffer for generating file names
    char filename[50];

    // current image index
    unsigned int idx = 0;

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

    // config file
    FILE * config;

    config = fopen(configFilename,"r");

    int camResX, camResY;
    int camFocal;

    int projectorRes;
    int projectorFocal;
    double pPosX, pPosY, pPosZ;
    double pOriX, pOriY, pOriZ;
    char imPath[128];
    int ignoreBits;

    //======= start reading config file =======//

    /*
    if(!fscanf(config,"#CAMERA")){
        printf("Syntax error: missing \"#CAMERA\"\n");
        return;
    }*/
    if(!fscanf(config,"resolution %dx%d\n",&camResX,&camResY)){
        printf("Syntax error: missing \"resolution ___x___\"\n");
        return;
    }
    if(!fscanf(config,"focal %d\n",&camFocal)){
        printf("Syntax error: missing \"focal ___ (camera)\"\n");
        return;
    }/*
    if(!fscanf(config,"#PROJECTOR")){
        printf("Syntax error: missing \"#PROJECTOR\"\n");
        return;
    }*/
    if(!fscanf(config,"resolution %d\n",&projectorRes)){
        printf("Syntax error: missing \"resolution ___\"\n");
        return;
    }
    if(!fscanf(config,"focal %d\n",&projectorFocal)){
        printf("Syntax error: missing \"focal ___ (projector)\"\n");
        return;
    }
    if(!fscanf(config,"posn %lf %lf %lf\n",&pPosX,&pPosY,&pPosZ)){
        printf("Syntax error: missing \"posn __ __ __\"\n");
        return;
    }
    if(!fscanf(config,"orin %lf %lf %lf\n",&pOriX,&pOriY,&pOriZ)){
        printf("Syntax error: missing \"orin __ __ __\"\n");
        return;
    }/*
    if(!fscanf(config,"#INPUT")){
        printf("Syntax error: missing \"#INPUT\"\n");
        return;
    }*/

    if(!fscanf(config,"ignore %d\n",&ignoreBits)){
        printf("Warning: missing \"ignore __\"\n");
        ignoreBits = 0;
    }
    if(!fscanf(config,"images %[^\r\n]s\n",imPath)){
        printf("Syntax error: missing \"images __________\"\n");
        return;
    }

    //======== end reading config file ========//
    printf("Camera resolution %dx%d\n",camResX,camResY);
    printf("Camera focal length %d\n", camFocal);
    printf("Projector resolution %d\n",projectorRes);
    printf("Projector focal length %d\n",projectorFocal);
    printf("Projector position %f %f %f\n",pPosX,pPosY,pPosZ);
    printf("Projector orientation %f %f %f\n",pOriX,pOriY,pOriZ);
    printf("Image path \"%s\"\n",imPath);
    printf("Ignore %d bits\n",ignoreBits);

    // set up the camera
    camera->setResolution(camResX, camResY);
    camera->setFocalLength(camFocal);
    camera->setPrincipalPoint(camResX/2, camResY/2);
    camera->setPosition(cv::Vec3d());

    // set up the projector
    //projectorRes = projectorRes >> ignoreBits;
    //projectorFocal = projectorFocal >> ignoreBits;
    projector->setResolution(projectorRes,1);
    projector->setFocalLength(projectorFocal);
    projector->setPrincipalPoint(projectorRes/2,0);
    projector->setPosition(cv::Vec3d(pPosX,pPosY,pPosZ));
    projector->setOrientation(cv::Vec3d(pOriX,pOriY,pOriZ));

    // determine the number of bits we need to project
    nmax = (unsigned int) ceil(log(projector->getResolutionU())/log(2));

    // initialize the quality mask
    qmask = cv::Mat::ones(camera->getResolution(),CV_32S);


    // construct the qmask filename
    sprintf(filename, imPath, 21);

    // load the frame
    img = cv::imread(filename,CV_LOAD_IMAGE_GRAYSCALE);
    img.convertTo(img,CV_32S);

    // construct the next filename
    sprintf(filename, imPath, 22);

    // load the frame
    inv = cv::imread(filename,CV_LOAD_IMAGE_GRAYSCALE);
    inv.convertTo(inv,CV_32S);

    // take the difference between the images
    qmask = inv - img;

    // initialize the encoded/decoded matrices
    encoded = cv::Mat::zeros(camera->getResolution(),CV_32S);
    decoded = cv::Mat::zeros(camera->getResolution(),CV_64F);

    // loop through the images
    for(n=0;n<nmax;n++){

        // construct the next filename
        sprintf(filename, imPath, idx++);

        // load the frame
        img = cv::imread(filename,CV_LOAD_IMAGE_GRAYSCALE);
        img.convertTo(img,CV_32S);
        printf("%s\n",filename);

        // construct the next filename
        sprintf(filename, imPath, idx++);
        printf("%s\n",filename);

        // load the frame
        inv = cv::imread(filename,CV_LOAD_IMAGE_GRAYSCALE);
        inv.convertTo(inv,CV_32S);

        // take the difference between the images
        img = inv - img;

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

    int bitVal;

    FILE *log;

    log = fopen("/home/ryan/Documents/mqp-data/samplerow.csv","w");

    // loop through every pixel
    for(x=0; x<img.cols; x++){
        for(y=0; y<img.rows; y++){
            if(abs(qmask.at<int>(y,x)) > QUALITY_THRESHOLD){
                bitVal = Triangulator::grayToBinary(
                            encoded.at<int>(y,x));
                bitVal = bitVal >> ignoreBits;
                bitVal = bitVal << ignoreBits;
                decoded.at<double>(y,x) = (double)bitVal;
            }else{
                decoded.at<double>(y,x) = -1;
            }
        }
    }

    cv::imwrite("/home/ryan/Documents/unbinary.png",decoded);

    std::vector<cv::Mat> fringes;

    // loop through the images again for fringes
    for(n=25;n<25+(1<<ignoreBits);n++){

        // construct the next filename
        sprintf(filename, imPath, n);

        // load the frame
        img = cv::imread(filename,CV_LOAD_IMAGE_GRAYSCALE);
        fringes.push_back(img);
    }

    cv::Mat phase = cv::Mat::zeros(fringes[0].rows,fringes[0].cols,
                                   CV_64F);
    Triangulator::computePhase(fringes,phase,1);
    cv::imwrite("/home/ryan/Documents/wrapped.png",phase*255);

    for(x=400; x<600; x++){
        fprintf(log,"%f\t%f\t%f\n",decoded.at<double>(400,x),phase.at<double>(400,x)*8,decoded.at<double>(400,x)+phase.at<double>(400,x)*8);
    }

    decoded = decoded + phase*8;

    cv::imwrite("/home/ryan/Documents/unwrapped.png",decoded);
    printf("\n");

    double px;
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

    uint decimation=3;

    Sheet sheet(cv::Size(img.cols,img.rows));

    for(x=0; x<img.cols-decimation; x+=decimation){
        for(y=0; y<img.rows-decimation; y+=decimation){
            px = decoded.at<double>(y,x);
            if(px<0) continue;
            P_fwd = projector->getPixelRay(px,0);
            M_hat = camera->getPixelRay(x,y);
            M = Triangulator::sumTo(M_hat,P_up,P_fwd,D);
            pt = cv::Point3d(M[0],M[1],M[2]);
            //cloud.add(pt);

            sheet.setPoint(x/decimation,y/decimation,M);
            if(     (decoded.at<double>(y+decimation,x)>0)&&
                    (decoded.at<double>(y,x+decimation)>0)&&
                    (decoded.at<double>(y+decimation,x+decimation)>0)){
                sheet.enableQuad(x/decimation,y/decimation);
            }

        }
    }

    sheet.saveSTL("output.stl");
    fclose(log);
    //cloud.meshlab("/home/ryan/Documents/mqp-data/head.obj");

}

cv::Mat TestMesher::calcPhase(FileSource source, uint m)
{
    cv::Mat out;
    return out;
}

#define HUE_FORMAT CV_8U

void TestMesher::writeToHueFile(char *fnm, cv::Mat img, double scale)
{
    std::vector<cv::Mat> hsv;
    hsv.resize(3);
    img.convertTo(img,CV_32S);
    hsv[0] = img*180/scale;
    hsv[1] = cv::Mat::ones(img.rows,img.cols,HUE_FORMAT)*255;
    hsv[2] = cv::Mat::ones(img.rows,img.cols,HUE_FORMAT)*255;
//    cv::threshold(image,hsv[2],0,255,CV_THRESH_BINARY);
    for(uint x=0; x<img.cols; x++){
        for(uint y=0; y<img.rows; y++){
            hsv[2].at<char>(y,x) = img.at<int>(y,x)>0?255:0;
        }
    }
    hsv[0].convertTo(hsv[0],HUE_FORMAT);
    hsv[2].convertTo(hsv[2],HUE_FORMAT);

    cv::Mat out;
    cv::merge(hsv,out);
    cv::cvtColor(out,out,CV_HSV2BGR);
    cv::imwrite(fnm,out);

}
