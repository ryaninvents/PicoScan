#ifndef CALIBRATIONSTANDARD_H
#define CALIBRATIONSTANDARD_H

#include <vector>
#include <opencv2/core/core.hpp>
#include "../camera/qcamera.h"

/// A generic, planar calibration standard.
class CalibrationStandard
{
public:
    /// Create a new standard.
    CalibrationStandard();

    /** Obtain a vector of the points of interest on this board. */
    virtual std::vector<cv::Point3f> getObjectPoints() = 0;

    /** Obtain a vector of vectors of POI on this board. */
    std::vector<std::vector<cv::Point3f> > getObjectPoints(unsigned int n);

    /** Find the points in the image corresponding to this standard.

        The order of these points is the same as those returned by
        getObjectPoints().

        \param image the image to search.
        \param success \a true if the standard was found,
                        \a false otherwise.
        \returns an ordered list of all points in the image. */
    virtual std::vector<cv::Point2f> getImagePoints(const cv::Mat image,
                                                    bool *success) = 0;

    /** Locate the standard in every image. */
    std::vector<std::vector<cv::Point2f> >
            getImagePoints(std::vector<cv::Mat> images,
                           bool *success);

    /** Calibrate a single camera. */
    bool calibrate(QCamera *camera,
                   std::vector<cv::Mat> images,
                   std::vector<cv::Mat> rvecs,
                   std::vector<cv::Mat> tvecs,
                   double *rpe);

    /// Get the first corner of the board.
    virtual int getPointA() = 0;
    /// Get the second corner of the board.
    virtual int getPointB() = 0;
    /// Get the third corner of the board.
    virtual int getPointC() = 0;
    /// Get the fourth corner of the board.
    virtual int getPointD() = 0;

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
