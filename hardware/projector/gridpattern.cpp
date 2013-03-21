#include "gridpattern.h"

GridPattern::GridPattern(uint spacing, uint width):
    _spacing(spacing),
    _w(width)
{
}

QImage GridPattern::generatePattern(int width, int height)
{
    QImage out(width,height,QImage::Format_ARGB32);
    int x,y;
    uint white = qRgba(255,255,255,255);
    uint black = qRgba(0,0,0,255);

    for (y = 0; y < height; y++) {
            QRgb *destrow = (QRgb*)out.scanLine(y);
            for (x = 0; x < width; ++x) {
                destrow[x] = ((y%_spacing<_w) || (x%_spacing<_w))?white:black;
            }
    }

    return out;
}
