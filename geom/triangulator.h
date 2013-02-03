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
};

#endif // TRIANGULATOR_H
