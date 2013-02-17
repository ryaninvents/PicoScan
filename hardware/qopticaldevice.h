#ifndef QOPTICALDEVICE_H
#define QOPTICALDEVICE_H

#include <QObject>
#include <opencv2/core/core.hpp>
/**
  Represents a generic optical device.
  This class models position, orientation, focal distance,
  retina size, aberrations, and other items common to both
  cameras and projectors.
  */
class QOpticalDevice : public QObject
{
    Q_OBJECT
public:
    /// Create a new QOpticalDevice.
    explicit QOpticalDevice(QObject *parent = 0);

    /** Sets the distortion vector for this device. */
    void setDistortion(cv::Mat k);

    /** Sets the 3×3 intrinsic matrix for this device.
        This matrix is of the form:
        <table>
            <tr><td>f<sub>x</sub></td>
                <td>α<sub>c</sub></td>
                <td>c<sub>x</sub></td></tr>
            <tr><td>0</td>
                <td>f<sub>y</sub></td>
                <td>c<sub>y</sub></td></tr>
            <tr><td>0</td>
                <td>0</td>
                <td>1</tr>
        </table>
        */
    void setIntrinsics(cv::Mat m);

    /// Update the orientation of this device with a
    /// Rodrigues vector.
    void setOrientation(cv::Mat o);
    /// Update the orientation of this device.
    void setOrientation(cv::Vec3d r);

    /// Update the position of this device.
    void setPosition(cv::Vec3d p);

    /// Set the resolution of this device.
    void setResolution(int u,int v);

    /// Set the resolution of this device.
    void setResolution(cv::Size s);

    /// Returns the distortion vector for this device.
    cv::Mat getDistortion();

    /// Returns the intrinsic matrix for this device.
    /// Distortions, while intrinsic, are not handled here.
    cv::Mat getIntrinsics();

    /// Get the horizontal component of a vector with
    /// a unit forward component and passing through
    /// the image point <tt>(u,0)</tt>.
    double getNormalizedU(double u);

    /// Get the vertical component of a vector with
    /// a unit forward component and passing through
    /// the image point <tt>(0,v)</tt>.
    double getNormalizedV(double v);

    /// Returns the orientation of this device as a
    /// <a href="http://en.wikipedia.org/wiki/Rodrigues'_rotation_formula">
    /// Rodrigues rotation matrix</a>.
    cv::Mat getOrientation();

    /// Returns a unit vector which passes through this device's origin
    /// and the given pixel in its array.
    cv::Mat getPixelRay(double u,double v);

    /// Returns a unit vector pointing in this device's "up" direction;
    /// i.e., in the direction of increasing <tt>v</tt>.
    cv::Mat getUpVector();

    /// Returns a unit vector pointing in this device's "forward" direction;
    /// i.e., the local <tt>z</tt> axis.
    cv::Mat getFwdVector();

    /// Returns the position in 3d space of this device.
    cv::Vec3d getPosition();

    /// Returns the horizontal resolution of this device.
    int getResolutionU();

    /// Returns the vertical resolution of this device.
    int getResolutionV();

    /// Returns the resolution of this device.
    cv::Size getResolution();

    /// Returns the focal length of the camera as a multiple of the
    /// physical pixel size; i.e., for an 8.0 μm cell size, if this method
    /// returns 1000 then the focal length is 8.0 mm.
    double getFocalLength();

    /// Sets the intrinsic matrix to the desired focal length. */
    void setFocalLength(double f);

    /// Set the principal point of the device.
    void setPrincipalPoint(double u, double v);

    /// Get the principal point U
    double getPrincipalU();

    /// Get the principal point V
    double getPrincipalV();
signals:
    
public slots:
private:

    /// Distortion coefficients
    cv::Mat distortion;

    /// Orientation as a Rodrigues vector
    cv::Mat orientation;

    /// Intrinsic matrix as described in documentation for setIntrinsics()
    cv::Mat intrinsicMatrix;

    /// Position
    cv::Vec3d position;

    /// Resolution of the device
    cv::Size resolution;

    /// Get a normalized coordinate
    double getNormalizedCoord(double u,double c);
    
};

#endif // QOPTICALDEVICE_H
