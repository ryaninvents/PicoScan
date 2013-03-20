#ifndef SURFEL_H
#define SURFEL_H

#include <opencv2/core/core.hpp>

/// Represents a <i>surfel</i>, or <i>surface element</i>.
class Surfel
{
public:
    Surfel();

private:
    // Location before applying orientation of model
    cv::Vec3d location;
    // Normal before applying orientation of model
    cv::Vec3d normal;
    // Model orientation
    cv::Mat modelOrientation;
    // Radius
    double radius;
    // Color
    cv::Vec3d color;
};

#endif // SURFEL_H
