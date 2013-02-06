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
                    M_hat[2],   P_up[2],  P_fwd[2]);* /
    A.col(0) = cv::Mat(M_hat).reshape(1);
    A.col(1) = cv::Mat(P_up).reshape(1);
    A.col(2) = cv::Mat(P_fwd).reshape(1);*/
    for(uint i=0;i<3;i++){
        A.at<double>(i,0) = M_hat[i];
        A.at<double>(i,1) = P_up[i];
        A.at<double>(i,2) = P_fwd[i];
    }

    cv::Mat D_mat = cv::Mat(D).reshape(1);

    // invert the matrix
    A = A.inv();

    // obtain the solution
    k = A * D_mat;

    // scale the output
    M = k.at<double>(0,0) * M_hat;

    return M;
}

bool Triangulator::inTri(const cv::Vec3d P, const cv::Vec3d A, const cv::Vec3d B, const cv::Vec3d C)
{
    // Compute vectors
    cv::Vec3d v0 = C - A;
    cv::Vec3d v1 = B - A;
    cv::Vec3d v2 = P - A;

    // Compute dot products
    double dot00 = v0.dot(v0);
    double dot01 = v0.dot(v1);
    double dot02 = v0.dot(v2);
    double dot11 = v1.dot(v1);
    double dot12 = v1.dot(v2);

    // Compute barycentric coordinates
    double invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
    double u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    double v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    // Check if point is in triangle
    return (u >= 0) && (v >= 0) && (u + v < 1);
}
