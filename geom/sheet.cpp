#include "sheet.h"

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
    cv::Point3d pt;
    for(u=0;u<cloud.cols;u++){
        for(v=0;v<cloud.rows;v++){
            if(quads.at<bool>(u,v)){
                ct++;
                pt = cloud.at<cv::Point3d>(u,v);
                x += pt.x;
                y += pt.y;
                z += pt.z;
            }
        }
    }
    if(ct==0) return cv::Point3d();
    return cv::Point3d(x/ct,y/ct,z/ct);
}

