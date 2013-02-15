#include "sheet.h"

#include <stdio.h>

Sheet::Sheet(cv::Size size)
{
    initialize(size,false);
}

Sheet::Sheet(cv::Size size, bool useColor)
{
    initialize(size,useColor);
}

void Sheet::initialize(cv::Size size, bool useColor)
{
    cloud = cv::Mat3d(size);
    quads = cv::Mat_<bool>(size);
    if(useColor){
        color = cv::Mat3d(size);
    }else{
        color = cv::Mat3d();
    }
}

cv::Point3d Sheet::getCentroid()
{
    int u,v,ct=0;
    double x=0,y=0,z=0;
    cv::Vec3d pt;
    for(u=0;u<cloud.cols;u++){
        for(v=0;v<cloud.rows;v++){
            if(quads.at<bool>(u,v)){
                ct++;
                pt = cloud.at<cv::Vec3d>(u,v);
                x += pt[0];
                y += pt[1];
                z += pt[2];
            }
        }
    }
    if(ct==0) return cv::Point3d();
    return cv::Point3d(x/ct,y/ct,z/ct);
}

void Sheet::setPoint(unsigned int u, unsigned int v, cv::Vec3d pt)
{
    cloud.at<cv::Vec3d>(u,v) = pt;
}

void Sheet::saveSTL(char *fnm)
{
    FILE *file;
    file = fopen(fnm,"w");
    cv::Vec3d ptA,ptB,ptC;

    fprintf(file,"solid\n");
    int u,v;
    for(u=0;u<cloud.cols-1;u++){
        for(v=0;v<cloud.rows-1;v++){
            if(quads.at<bool>(u,v)){
                ptA = cloud.at<cv::Vec3d>(u,v);
                ptB = cloud.at<cv::Vec3d>(u,v+1);
                ptC = cloud.at<cv::Vec3d>(u+1,v);
                if(ptA[2]-ptC[2]>1 || ptA[2]-ptC[2]<-1) continue;
                fprintf(file, "facet normal 0 0 0\n");
                fprintf(file, " outer loop\n");
                fprintf(file, "  vertex %f %f %f\n", ptA[0],ptA[1],ptA[2]);
                fprintf(file, "  vertex %f %f %f\n", ptB[0],ptB[1],ptB[2]);
                fprintf(file, "  vertex %f %f %f\n", ptC[0],ptC[1],ptC[2]);
                fprintf(file, " endloop\nendfacet\n");
                ptA = cloud.at<cv::Vec3d>(u+1,v+1);
                fprintf(file, "facet normal 0 0 0\n");
                fprintf(file, " outer loop\n");
                fprintf(file, "  vertex %f %f %f\n", ptB[0],ptB[1],ptB[2]);
                fprintf(file, "  vertex %f %f %f\n", ptA[0],ptA[1],ptA[2]);
                fprintf(file, "  vertex %f %f %f\n", ptC[0],ptC[1],ptC[2]);
                fprintf(file, " endloop\nendfacet\n");
            }
        }
    }

    fprintf(file,"endsolid");

    fclose(file);
}

void Sheet::enableQuad(unsigned int u, unsigned int v)
{
    quads.at<bool>(u,v) = true;
}

Sheet Sheet::decimate(int n)
{
    cv::Size nuSize(cloud.rows/(n-1),cloud.cols/(n-1));
    cv::Mat3d nuCloud(nuSize);
    cv::Mat_<bool> nuQuads(nuSize);
    uint u,v;
    for(u=0;u<nuSize.width;u++){
        for(v=0;v<nuSize.height;v++){
            nuCloud.at<cv::Vec3d>(u,v) = cloud.at<cv::Vec3d>(u*n,v*n);
            nuQuads.at<bool>(u,v) = quads.at<bool>(u*n,v*n);
        }
    }
    cloud = nuCloud;
    quads = nuQuads;
}

