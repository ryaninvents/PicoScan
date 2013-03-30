#include "chessboardstandard.h"

#include <opencv2/calib3d/calib3d.hpp>

ChessboardStandard::ChessboardStandard(cv::Size size, double scale):
    size(size),
    scaleX(scale),
    scaleY(scale)
{
}

ChessboardStandard::ChessboardStandard(cv::Size size, double scaleX, double scaleY):
    size(size),
    scaleX(scaleX),
    scaleY(scaleY)
{
}

std::vector<cv::Point2f> ChessboardStandard::getImagePoints(
        const cv::Mat image,
        bool *success)
{
    std::vector<cv::Point2f> corners;
    *success = cv::findChessboardCorners(image,
                              size,
                              corners);
    return corners;
}

std::vector<cv::Point3f> ChessboardStandard::getObjectPoints()
{
    int y,x;
    double _y, _x;
    std::vector<cv::Point3f> pts;

    for(y=0;y<size.height;y++){
        for(x=0;x<size.width;x++){
            _y = y;
            _x = x;
            _y *= scaleY;
            _x *= scaleX;
            pts.push_back(cv::Point3f(_x,_y,0));
        }
    }
    return pts;
}

int ChessboardStandard::getPointA()
{
    return 0;
}

int ChessboardStandard::getPointB()
{
    return size.width - 1;
}

int ChessboardStandard::getPointC()
{
    return (size.height-1)*size.width;
}

int ChessboardStandard::getPointD()
{
    return (size.width*size.height)-1;
}
