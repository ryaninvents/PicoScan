#ifndef SHEET_H
#define SHEET_H

#include <vector>

#include <opencv2/core/core.hpp>
#include <QtOpenGL>

/**
  Represents a "sheet", a mesh generated from a single view. Because it was
  generated from a photograph, we know there's a very strong relationship
  between the \c (u,v) coordinates of the photograph and the \c (x,y,z)
  coordinates of the real-world object. This makes it much easier to link up the
  point cloud into a mesh. Sheet takes advantage of this fact and represents the
  mesh as a 2-dimensional matrix, each entry of which is a 3-dimensional point.

      \todo Create a method
            which returns the 3d location of a point with the given \c (u,v)
            coordinates. cv::Point3d getPoint(cv::Point2d)
      \todo Create a method which accepts a 2d point as a parameter and returns
            a vector of 2d points indicating a loop (or a vector of zero length
            if no such loop exists or multiple such loops exist). This will
            enable the software to eventually be able to "heal" small holes.
  */
class Sheet
{
public:
    /**
      Create a new Sheet with the given size, not using color data.
      */
    Sheet(cv::Size size);

    /**
      Create a new Sheet with the given size, using color data if \b useColor
      is \c true.
      \param size the (u,v) size of the sheet
      \param useColor if \c true, then the Sheet will also contain color data;
                      if \c false, no color data will be stored.
      */
    Sheet(cv::Size size, bool useColor);

    /**
      Return the geometry as a list of points.
      This is useful for visualizing the data as a "raw point cloud."
      */
    std::vector<GLdouble> getPoints();

    /**
      Return the geometry as lists of points, ordered suitably for
      OpenGL rendering. This will allow the surface to be rendered
      as a surface.
      */
    //std::vector<std::vector<GLdouble> > getQuadStrips(int decimation);

    /**
      Reduce the resolution of the mesh, primarily to improve response time of a
      3D display. Each square of the new mesh will have a side length of roughly
      \b n times that of the old mesh.

      \param n the factor by which to decimate the mesh

      \returns a new Sheet representing a lower-resolution version of this one
      */
    Sheet decimate(int n);

    /**
      Get the centroid of the mesh. This may be used for neat effects in the
      future where the point cloud "condenses" into being right in front of your
      eyes.

      This is only an approximation, since for a visual effect we don't need
      precision.

      \returns The (approximate) average of all the points in this Sheet.
      */
    cv::Point3d getCentroid();

    /// Add a point to the sheet.
    void setPoint(unsigned int u, unsigned int v, cv::Vec3d pt);

    /// Save to STL format.
    void saveSTL(char *fnm);

    /// Get a particular point.
    cv::Vec3d getPoint(uint u, uint v);

    /// Does the mesh have a point at the given (u,v)?
    bool hasPointAt(uint u, uint v);

    /// Get the parametric width
    uint getWidth();
    /// Get the parametric height
    uint getHeight();



private:
    /**
      The 3D location of each point on the sheet.
      Points not represented by the sheet may have any value.
      Must be the same size as \b quads and \b color (if \b color
      has any value at all).
      */
    cv::Mat3d cloud;
    /**
      Determines which points are actually on the sheet.
      A value of \c true in a given position means that the matching point
      from \b cloud is a corner of a quad. This roughly acts
      in a similar manner to an alpha value.
      Must be the same size as \b cloud (and \b color if it exists).
      */
    cv::Mat_<bool> alpha;
    /**
      The RGB color of each point.
      May remain uninitialized if not needed.
      */
    cv::Mat3d color;

    /// Set up the matrices
    void initialize(cv::Size size, bool useColor);
};

#endif // SHEET_H
