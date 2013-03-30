#ifndef GRIDPATTERN_H
#define GRIDPATTERN_H

#include "qprojector.h"

class GridPattern : public QProjector::Pattern
{
public:
    GridPattern(uint spacing = 50, uint width=1);
    QImage generatePattern(int width, int height);
private:
    uint _spacing;
    uint _w;
};

#endif // GRIDPATTERN_H
