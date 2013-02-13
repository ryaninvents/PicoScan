#include "dotmatrixstandard.h"

#include <opencv2/calib3d/calib3d.hpp>

DotMatrixStandard::DotMatrixStandard(cv::Size size,
                                     double scaleX,
                                     double scaleY,
                                     double offset):
    size(size),
    scaleX(scaleX),
    scaleY(scaleY),
    offset(offset)
{
}

DotMatrixStandard::DotMatrixStandard(cv::Size size, double scale):
    size(size),
    scaleX(scale),
    scaleY(scale),
    offset(0)
{
}

std::vector<cv::Point2f> DotMatrixStandard::getImagePoints(
        cv::Mat image,
        bool *success)
{
    std::vector<cv::Point2f> corners;
    *success = cv::findCirclesGrid(image,size,corners,
                                   offset<0.000001?
                                       cv::CALIB_CB_SYMMETRIC_GRID:
                                       cv::CALIB_CB_ASYMMETRIC_GRID);
    return corners;
}

std::vector<cv::Point3f> DotMatrixStandard::getObjectPoints()
{
    return getGridPoints(size,scaleX,scaleY,offset);
}


int DotMatrixStandard::getPointA()
{
    return 0;
}

int DotMatrixStandard::getPointB()
{
    return size.height - 1;
}

int DotMatrixStandard::getPointC()
{
    return (size.width-1)*size.height;
}

int DotMatrixStandard::getPointD()
{
    return (size.width*size.height)-1;
}
