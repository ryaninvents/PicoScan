#include "chessboardstandard.h"

#include <opencv2/calib3d/calib3d.hpp>

ChessboardStandard::ChessboardStandard(cv::Size size, double scale):
    size(size),
    scale(scale)
{
}

std::vector<cv::Point2f> ChessboardStandard::getImagePoints(
        cv::Mat image,
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
    int x,y;
    double _x, _y;
    std::vector<cv::Point3f> pts;

    for(x=0;x<size.height;x++){
        for(y=0;y<size.width;y++){
            _x = x;
            _y = y;
            _x *= scale;
            _y *= scale;
            pts.push_back(cv::Point3f(_y,_x,0));
        }
    }
    return pts;
}
