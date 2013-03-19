#ifndef DOTMATRIXSTANDARD_H
#define DOTMATRIXSTANDARD_H

#include "calibrationstandard.h"
#include <iostream>

/// A dot-matrix standard consisting of a grid of dots.
/// The dots may form a regular grid, or they may form
/// a "staggered" grid by offsetting every other row by
/// a constant amount.
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
    /// Create a new dot matrix standard.
    DotMatrixStandard(cv::Size size, double scaleX, double scaleY, double offset);
    /// Create a new, regular, standard.
    DotMatrixStandard(cv::Size size, double scale);

    /// Find standard points in an image.
    std::vector<cv::Point2f> getImagePoints(const cv::Mat image, bool *success);

    /// Get a list of the points in this standard.
    std::vector<cv::Point3f> getObjectPoints();

    /// Find the index of the first corner.
    int getPointA();
    /// Find the index of the second corner.
    int getPointB();
    /// Find the index of the third corner.
    int getPointC();
    /// Find the index of the fourth corner.
    int getPointD();
};

#endif // DOTMATRIXSTANDARD_H
