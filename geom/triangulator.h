#ifndef TRIANGULATOR_H
#define TRIANGULATOR_H

#include <opencv2/core/core.hpp>
#include "../hardware/camera/qcamera.h"
#include "sheet.h"

/// Provides triangulation as well as other utilities.
class Triangulator
{
public:
    /// Given a line <tt>L = M_hat * s</tt>,
    /// and a plane <tt>P = D + P_up * s + P_fwd * t</tt>,
    /// find the point of intersection.
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

    /** Get a best-fit plane for a set of points.

        The function returns a vector Q. The equation for
        the plane is x = Q.dot(cv::Vec3d(y,z,1)).*/
    static cv::Vec3d getPlane(std::vector<cv::Vec3d> pts);

    /** Get the centroid of a set of pts. */
    static cv::Vec3d getCentroid(std::vector<cv::Vec3d> pts);

    /** Get the normal of a plane returned by getPlane(). */
    //static cv::Vec3d getPlaneNormal(cv::Vec3d plane);

    /// Compute wrapped phase from set of images.
    static void computePhase(std::vector<cv::Mat> fringes,
                             cv::Mat output,
                             double scale);

    /// Compute binary encoding from a set of images.
    static cv::Mat computeBinary(std::vector<cv::Mat> frames,
                                 uint maskThreshold,
                                 uint drop=0);

    /// Convert to HSV
    static cv::Mat maphsv(cv::Mat img,double scale);

    /// Convert binary to Gray code.
    static int binaryToGray(int num);
    /// Convert Gray code to binary.
    static int grayToBinary(int num);

    /// Generate a sheet
    static std::vector<cv::Vec3d> computeGeometry(cv::Mat encoding,
                                 QCamera *camera,
                                 QProjector *projector,
                                 uint decimation=1);

    /// Calibrate a single camera.

    /// Calibrate a stereo pair.
    static Sheet *computeSheet(cv::Mat encoding,
                               QCamera *camera,
                               QProjector *projector,
                               uint decimation);
};

#endif // TRIANGULATOR_H
