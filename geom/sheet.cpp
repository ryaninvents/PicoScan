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

std::vector<GLdouble> Sheet::getPoints()
{
    uint v,u;
    std::vector<GLdouble> pts;
    cv::Vec3d pt;
    for(u=0;u<cloud.cols;u++){
        for(v=0;v<cloud.rows;v++){
            if(!hasPointAt(u,v)) continue;
            pt = getPoint(u,v);
            pts.push_back((GLdouble)pt[0]);
            pts.push_back((GLdouble)pt[1]);
            pts.push_back((GLdouble)pt[2]);
        }
    }
    return pts;
}

void Sheet::initialize(cv::Size size, bool useColor)
{
    cloud = cv::Mat3d(size.height,size.width);
    alpha = cv::Mat_<bool>(size.height,size.width);
    if(useColor){
        color = cv::Mat3d(size.height,size.width);
    }else{
        color = cv::Mat3d();
    }
    width = size.width;
    height = size.height;
}

cv::Point3d Sheet::getCentroid()
{
    int v,u,ct=0;
    double x=0,y=0,z=0;
    cv::Vec3d pt;
    for(u=0;u<cloud.cols;u++){
        for(v=0;v<cloud.rows;v++){
            if(alpha.at<bool>(v,u)){
                ct++;
                pt = cloud.at<cv::Vec3d>(v,u);
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
    cloud.at<cv::Vec3d>(v,u) = pt;
    alpha.at<bool>(v,u) = true;
}

void Sheet::saveSTL(char *fnm)
{
    FILE *file;
    file = fopen(fnm,"w");
    cv::Vec3d ptA,ptB,ptC;

    fprintf(file,"solid\n");
    uint v,u;
    for(u=0;u<getWidth();u++){
        for(v=0;v<getHeight();v++){
            if(     hasPointAt(u,v) &&
                    hasPointAt(u+1,v) &&
                    hasPointAt(u,v+1)){
                ptA = getPoint(u,v);
                ptB = getPoint(u+1,v);
                ptC = getPoint(u,v+1);
                if(ptA[2]-ptC[2]>1 || ptA[2]-ptC[2]<-1) continue;
                fprintf(file, "facet normal 0 0 0\n");
                fprintf(file, " outer loop\n");
                fprintf(file, "  vertex %f %f %f\n", ptA[0],ptA[1],ptA[2]);
                fprintf(file, "  vertex %f %f %f\n", ptB[0],ptB[1],ptB[2]);
                fprintf(file, "  vertex %f %f %f\n", ptC[0],ptC[1],ptC[2]);
                fprintf(file, " endloop\nendfacet\n");
                if(!hasPointAt(u+1,v+1)) continue;
                ptA = getPoint(u+1,v+1);
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

cv::Vec3d Sheet::getPoint(uint u, uint v)
{
    return cloud.at<cv::Vec3d>(v,u);
}

bool Sheet::hasPointAt(uint u, uint v)
{
    if(v>=alpha.rows || u>=alpha.cols) return false;
//    cv::Vec3d pt = cloud.at<cv::Vec3d>(v,u);
//    if(     (abs(pt[0])<1e-10) &&
//            (abs(pt[1])<1e-10) &&
//            (abs(pt[2])<1e-10)) return false;
    return alpha.at<bool>(v,u);
}

uint Sheet::getWidth()
{
    return cloud.cols;
}

uint Sheet::getHeight()
{
    return cloud.rows;
}

void Sheet::removeNonManifold()
{
    cv::Mat_<bool> remove(cloud.rows,cloud.cols);
    uint u,v;
    for(u=0;u<remove.cols;u++){
        for(v=0;v<remove.rows;v++){
            if(     !hasPointAt(u-1,v) ||
                    !hasPointAt(u+1,v) ||
                    !hasPointAt(u,v-1) ||
                    !hasPointAt(u,v+1)){
                remove.at<bool>(v,u) = true;
            }else remove.at<bool>(v,u) = false;
        }
    }
    for(u=0;u<remove.cols;u++){
        for(v=0;v<remove.rows;v++){
            if(remove.at<bool>(v,u)){
                alpha.at<bool>(v,u) = false;
            }
        }
    }
}

Sheet Sheet::decimate(int n)
{
    Sheet sheet(cv::Size(getWidth()/n+1,getHeight()/n+1));
    uint v,u;
    for(u=0;u<getWidth();u+=n){
        for(v=0;v<getHeight();v+=n){
            sheet.setPoint(u/n,v/n,getPoint(u,v));
        }
    }
    return sheet;
}

