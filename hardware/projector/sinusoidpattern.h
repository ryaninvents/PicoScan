#ifndef SINUSOIDPATTERN_H
#define SINUSOIDPATTERN_H

#include "qprojector.h"

/// Represents a sinusoid pattern.

class SinusoidPattern : public QProjector::Pattern
{
public:
    /// Create the pattern
    SinusoidPattern(uint width,
                    uint shift,
                    bool horiz = false);
    /// Generate the pattern to project.
    QImage generatePattern(int width, int height);
    /// Get the ID for this pattern.
    unsigned int getPatternID();

    uint getPeriod();
    uint getShift();
    void setBrightness(uint b);

private:
    /// Fringe width (px)
    uint period;
    /// Fringe shift (px)
    uint shift;
    /// Is the pattern horizontal?
    bool horiz;
    uint brightness;

    /// Get sinusoid value
    uint getSinusoidValue(uint x);
};

#endif // SINUSOIDPATTERN_H
