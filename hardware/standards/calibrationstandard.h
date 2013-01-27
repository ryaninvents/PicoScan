#ifndef CALIBRATIONSTANDARD_H
#define CALIBRATIONSTANDARD_H

#include <vector>
#include <opencv2/core/core.hpp>
#include "../camera/camera.h"

class CalibrationStandard
{
public:
    CalibrationStandard();

    /** Obtain a vector of the points of interest on this board. */
    virtual std::vector<cv::Point3d> getObjectPoints() = 0;

    std::vector<std::vector<cv::Point3d> > getObjectPoints(unsigned int n);

    /** Find the points in the image corresponding to this standard.

        The order of these points is the same as those returned by
        getObjectPoints().

        \param image the image to search.
        \param success \a true if the standard was found,
                        \a false otherwise.
        \returns an ordered list of all points in the image. */
    virtual std::vector<cv::Point2d> getImagePoints(cv::Mat image,
                                                    bool *success) = 0;

    /** Locate the standard in every image. */
    std::vector<std::vector<cv::Point2d> >
            getImagePoints(std::vector<cv::Mat> images,
                           bool *success);

    /** Calibrate a single camera. */
    bool calibrate(Camera *camera,
                   std::vector<cv::Mat> images,
                   std::vector<cv::Mat> rvecs,
                   std::vector<cv::Mat> tvecs,
                   double *rpe);

protected:
    /** Utility function for quickly generating a grid.
      \param size The dimensions of the grid, in (cols, rows).
      \param scaleX Horizontal grid spacing.
      \param scaleY Vertical grid spacing.
      \param offset Set this to create staggered rows, or to zero for a uniform
                        grid.
    */
    static std::vector<cv::Point3f> getGridPoints(cv::Size size,
                                                  double scaleX,
                                                  double scaleY,
                                                  double offset);
};

#endif // CALIBRATIONSTANDARD_H
