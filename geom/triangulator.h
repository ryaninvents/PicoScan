#ifndef TRIANGULATOR_H
#define TRIANGULATOR_H

#include <QObject>
#include <opencv2/core/core.hpp>

/// Provides triangulation as well as other utilities.
class Triangulator : public QObject
{
    Q_OBJECT
public:
    /// Create a Triangulator. Calling this constructor
    /// is fairly useless, as this class consists entirely
    /// of static methods, but I want to be able to subclass it.
    explicit Triangulator(QObject *parent = 0);

    /// Given a line <tt>L = M_hat * s</tt>,
    /// and a plane <tt>P = D + P_up * s + P_fwd * t</tt>,
    /// find the point of intersection.
    static cv::Vec3d sumTo(const cv::Vec3d M_hat,
                           const cv::Vec3d P_up,
                           const cv::Vec3d P_fwd,
                           const cv::Vec3d D);

    /// Find the intersection between a camera ray and a
    /// projector fringe.
    /// \param camera Pointer to the camera.
    /// \param u The u-value of the camera pixel
    /// \param v The v-value of the camera pixel
    /// \param projector Pointer to the projector.
    /// \param pU The u-value of the projector fringe.
    /*static cv::Vec3d intersect(
            QOpticalDevice *camera,
            double u, double v,
            QOpticalDevice *projector,
            double pU);*/

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
};

#endif // TRIANGULATOR_H
