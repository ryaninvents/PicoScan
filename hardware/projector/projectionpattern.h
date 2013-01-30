#ifndef PROJECTIONPATTERN_H
#define PROJECTIONPATTERN_H

#include <opencv2/core/core.hpp>

/**
  The type of pattern this ProjectionPattern represents.
  */
enum PatternType{
    /** Represents a single-pixel pattern. */
    PIXEL,
    /** Represents a horizontally-varying pattern.*/
    HORIZONTAL,
    /** Represents a vertically-varying pattern.*/
    VERTICAL,
    /** Represents a two-dimensionally-varying pattern.*/
    TWO_DIMENSIONAL
};

/**
  Represents a pattern that may be projected onto an object for scanning.
  */
class ProjectionPattern
{
public:
    /**
      Create a new ProjectionPattern with the desired dimensions.
      \param width the width of the pattern, or \c 0 to create a vertical
                pattern
      \param height the height of the pattern, or \c 0 to create a horizontal
                pattern
      */
    ProjectionPattern(unsigned int width, unsigned int height);

    /**
      Create a new horizontal ProjectionPattern with the desired width.
      \param width the width of the pattern
      */
    ProjectionPattern(unsigned int width);

    /**
      Generate the pattern into a matrix.
      */
    virtual cv::Mat getPattern() = 0;

    /** Get the type of pattern. */
    virtual PatternType getType() = 0;

    /** Get the size of the pattern. */
    cv::Size getSize(){return size;}
private:
    PatternType type;
    cv::Size size;

};

#endif // PROJECTIONPATTERN_H
