#ifndef FLATCOLORPATTERN_H
#define FLATCOLORPATTERN_H

#include "qprojector.h"

/// Represents a flat color being projected.

class FlatColorPattern : public QProjector::Pattern
{
public:
    /// Create a blank pattern
    FlatColorPattern();

    QImage generatePattern(int width, int height);

private:
    /// amount of red
    float r;
    /// amount of green
    float g;
    /// amount of blue
    float b;
    
};

#endif // FLATCOLORPATTERN_H
