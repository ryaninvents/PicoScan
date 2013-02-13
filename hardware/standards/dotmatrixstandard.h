#ifndef DOTMATRIXSTANDARD_H
#define DOTMATRIXSTANDARD_H

#include "calibrationstandard.h"

class DotMatrixStandard : public CalibrationStandard
{
private:

    /// The size of the matrix.
    cv::Size size;

    /// The horizontal distance between dot centers, in millimeters.
    double scaleX;

    /// The vertical distance between rows of dots, in millimeters.
    double scaleY;

    /// The offset of every other row.
    double offset;

public:
    DotMatrixStandard(cv::Size size, double scaleX, double scaleY, double offset);
    DotMatrixStandard(cv::Size size, double scale);

    std::vector<cv::Point2f> getImagePoints(cv::Mat image, bool *success);

    std::vector<cv::Point3f> getObjectPoints();

    int getPointA();
    int getPointB();
    int getPointC();
    int getPointD();
};

#endif // DOTMATRIXSTANDARD_H
