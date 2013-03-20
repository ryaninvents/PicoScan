#ifndef CHESSBOARDSTANDARD_H
#define CHESSBOARDSTANDARD_H

#include "calibrationstandard.h"

#include <vector>
#include <opencv2/core/core.hpp>

/// Represents a plain chessboard standard.
class ChessboardStandard : public CalibrationStandard
{
private:
    /** The size of the chessboard.
      Keep in mind that this counts square intersections, not squares. A
      regulation 8x8 chessboard, then, would have a @a size of 7x7. */
    cv::Size size;

    /// The width of a square, in millimeters.
    double scaleX;

    /// The height of a square, in millimeters.
    double scaleY;

public:
    /** Create a new chessboard.
        \param size The dimensions (width, height) of the chessboard. This
                counts intersections, so a regular 8x8 board would have a
                \a size of 7x7.
        \param scale The length of a square's side, in millimeters. */
    ChessboardStandard(cv::Size size, double scale);

    /// Create a new rectangular chessboard.
    ChessboardStandard(cv::Size size, double scaleX, double scaleY);

    /// Find the chessboard in the image.
    std::vector<cv::Point2f> getImagePoints(const cv::Mat image, bool *success);

    /// Get a list of points on this chessboard.
    std::vector<cv::Point3f> getObjectPoints();

    /// First corner
    int getPointA();
    /// Second corner
    int getPointB();
    /// Third corner
    int getPointC();
    /// Fourth corner
    int getPointD();


};

#endif // CHESSBOARDSTANDARD_H
