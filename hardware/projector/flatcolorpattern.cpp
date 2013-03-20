#include "flatcolorpattern.h"

FlatColorPattern::FlatColorPattern():
    r(0),
    g(0),
    b(0)
{
}

QImage FlatColorPattern::generatePattern(int width, int height)
{
    QImage out(width,height,QImage::Format_ARGB32);
    unsigned int x,y;
    uint red = (uint)(r*255);
    uint green = (uint)(g*255);
    uint blue = (uint)(b*255);
    QRgb color = qRgba(red,green,blue,255);

    for (y = 0; y < height; y++) {
            QRgb *destrow = (QRgb*)out.scanLine(y);
            for (x = 0; x < width; ++x) {
                destrow[x] = color;
            }
    }

    return out;
}
