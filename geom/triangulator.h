#ifndef TRIANGULATOR_H
#define TRIANGULATOR_H
#include <opencv2/core/core.hpp>
class Triangulator
{
public:
    Triangulator();
    static cv::Vec3d sumTo(const cv::Vec3d M_hat,
                           const cv::Vec3d P_up,
                           const cv::Vec3d P_fwd,
                           const cv::Vec3d D);
    /** Find out if point P is within triangle ABC.
        Thanks to <a href="http://www.blackpawn.com/texts/pointinpoly/default.html">
        blackpawn.com</a> for the code. */
    static bool inTri(const cv::Vec3d P,
                      const cv::Vec3d A,
                      const cv::Vec3d B,
                      const cv::Vec3d C);
};

#endif // TRIANGULATOR_H
