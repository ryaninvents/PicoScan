#include "triangulator.h"

Triangulator::Triangulator()
{
}

cv::Vec3d Triangulator::sumTo(const cv::Vec3d M_hat,
                              const cv::Vec3d P_up,
                              const cv::Vec3d P_fwd,
                              const cv::Vec3d D)
{
    // vector to measured point
    static cv::Vec3d M;

    // working matrix
    cv::Mat A = cv::Mat::zeros(3,3,CV_64F);

    // solution matrix
    cv::Mat k;


    // set up working matrix
    /*A = cv::Matx33d(M_hat[0],   P_up[0],  P_fwd[0],
                    M_hat[1],   P_up[1],  P_fwd[1],
                    M_hat[2],   P_up[2],  P_fwd[2]);*/
    A.col(0) = cv::Mat(M_hat).reshape(1).t();
    A.col(1) = cv::Mat(P_up).reshape(1).t();
    A.col(2) = cv::Mat(P_fwd).reshape(1).t();

    cv::Mat D_mat = cv::Mat(D).reshape(1).t();

    // invert the matrix
    A = A.inv();

    // obtain the solution
    k = A.mul(D_mat);

    // scale the output
    M = k.at<double>(0,0) * M_hat;

    return M;
}
