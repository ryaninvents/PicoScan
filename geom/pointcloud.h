#ifndef FR_DATA_POINTCLOUD_H
#define FR_DATA_POINTCLOUD_H

#include <vector>
#include <opencv2/core/core.hpp>

class PointCloud
{
public:
    // default constructor containing no points
    PointCloud();

    // save the cloud to a Wavefront .obj file
    void saveObj(const char *fnm);

    // save the cloud to a Stanford .ply file
    void savePly(const char *fnm);

    // save the cloud to a Matlab .m file
    void saveMatlab(const char *fnm);

    // save and display in meshlab
    void meshlab(const char *fnm);

    // add a point to the cloud
    void add(cv::Point3d p);

    // add a whole bunch of points to the cloud
    void add(PointCloud cloud);

    // translate the origin
    void translateOrigin(cv::Point3d pt);

    // rotate about Z axis
    void rotateY(double angle);

    // how many points are in this cloud?
    unsigned int size();

    // get the ith point
    cv::Point3d get(int i);


    void rotateX(double angle);
private:
    std::vector<cv::Point3d> pts;
};


#endif // FR_DATA_POINTCLOUD_H
