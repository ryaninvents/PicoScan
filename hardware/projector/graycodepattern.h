#ifndef GRAYCODEPATTERN_H
#define GRAYCODEPATTERN_H

#include "qprojector.h"

/// Represents a "Gray code" projection pattern,
/// also known as "reflected binary".
/// Please note that this is not direct-coded
/// grayscale levels. If you're after that then
/// you're in for a world of hurt.
class GrayCodePattern : public QProjector::Pattern
{
public:
    /// Create a new Gray code pattern.
    GrayCodePattern();
    /// Generate the pattern to project.
    QImage generatePattern(int width, int height);
    /// Get the ID for this pattern.
    unsigned int getPatternID();
    /// Convert binary to Gray code.
    static int binaryToGray(int num);
    /// Convert Gray code to binary.
    static int grayToBinary(int num);

    /// Get the bit
    unsigned int getBit();

    /// Is the binary pattern inverted?
    bool isInverted();

private:
    /// Which bit this is.
    unsigned int bit;
    /// Is the image inverted?
    bool inverted;
};

#endif // GRAYCODEPATTERN_H
