#ifndef POVRAYPROJECTOR_H
#define POVRAYPROJECTOR_H

#include "qprojector.h"
#include <opencv2/core/core.hpp>

/// A projector you can use with POV-Ray!
/// In order for this to work, you need to have a
/// .pov file which includes the .inc file this
/// projector will write to. In addition, you
/// need to pass an image file that this projector
/// can write to. Then, when you request an image
/// from a PovRayCamera, POV-Ray will use the info
/// from the projector and the camera to render
/// the scene! Nifty, huh?
///
/// See PovRayCamera for more information and links
/// to sample model repositories.
///
/// \bug If you're going to try to simulate a 2D
/// pattern, this will only (accurately) project
/// square patterns.
/// For horizontally-varying patterns this is ok,
/// but for vertically- or 2D-varying patterns you'll
/// have to rewrite some of this code. Should be
/// simple, you'd just have to introduce an aspect
/// ratio scale factor when the PovRayProjector is
/// writing to the parameters file.
class PovRayProjector : public QProjector
{
    Q_OBJECT
public:
    /// Create a POV-Ray Projector.
    explicit PovRayProjector(QObject *parent = 0);

    /// Set the image filename
    void setFilterFilename(QString fnm);
    /// Set the parameters filename
    void setParamFilename(QString fnm);
    /// Get the filename where the pattern will be saved
    QString getFilterFilename();
    /// Set simulated position
    void setSimPosition(double x,double y,double z);
    /// Set simulated orientation
    void setSimRotation(double x,double y,double z);
    
signals:
    
public slots:

protected:
    /// Project a pattern.
    void projectPattern(QProjector::Pattern *pattern);

private:
    /// Filename where projection patterns are written to.
    QString imageFilename;

    /// Filename where projector parameters are written to.
    QString projFilename;

    /// Simulated position.
    cv::Vec3d simPosition;

    /// Simulated orientation, in Rodrigues notation.
    /// Basically, the vector represents an axis, and
    /// its length represents the amount to rotate.
    cv::Vec3d simRotation;

    /// Simulated cell size, you probably
    /// don't need to worry about this
    double simCellSize;

    /// Simulated focal
    double simFocal;

    /// Last projected pattern.
    QProjector::Pattern *lastProjected;

};

#endif // POVRAYPROJECTOR_H
