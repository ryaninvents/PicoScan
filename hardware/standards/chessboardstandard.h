#ifndef CHESSBOARDSTANDARD_H
#define CHESSBOARDSTANDARD_H

#include "calibrationstandard.h"

#include <vector>
#include <opencv2/core/core.hpp>

class ChessboardStandard : public CalibrationStandard
{
private:
    /** The size of the chessboard.
      Keep in mind that this counts square intersections, not squares. A
      regulation 8x8 chessboard, then, would have a @a size of 7x7. */
    cv::Size size;

    /** The length of a square size, in millimeters. */
    double scale;

public:
    /** Create a new chessboard.
        \param size The dimensions (width, height) of the chessboard. This
                counts intersections, so a regular 8x8 board would have a
                \a size of 7x7.
        \param scale The length of a square's side, in millimeters. */
    ChessboardStandard(cv::Size size, double scale);

    std::vector<cv::Point2f> getImagePoints(cv::Mat image, bool *success);

    std::vector<cv::Point3f> getObjectPoints();


    int getPointA();
    int getPointB();
    int getPointC();
    int getPointD();


};

#endif // CHESSBOARDSTANDARD_H
