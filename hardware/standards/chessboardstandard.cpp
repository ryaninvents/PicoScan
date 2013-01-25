#include "chessboardstandard.h"

#include <opencv2/calib3d/calib3d.hpp>

ChessboardStandard::ChessboardStandard(cv::Size size, double scale):
    size(size),
    scale(scale)
{
}

std::vector<cv::Point2d> ChessboardStandard::getImagePoints(
        cv::Mat image,
        bool *success)
{
    std::vector<cv::Point2d> corners;
    *success = cv::findChessboardCorners(image,
                              cv::Size(image.size[0],image.size[1]),
                              corners);
    return corners;
}

std::vector<cv::Point3d> ChessboardStandard::getObjectPoints()
{
    int x,y;
    double _x, _y;
    std::vector<cv::Point3d> pts;

    for(x=0;x<size.height;x++){
        for(y=0;y<size.width;y++){
            _x = x;
            _y = y;
            _x *= scale;
            _y *= scale;
            pts.push_back(cv::Point3d(_y,_x,0));
        }
    }
    return pts;
}
