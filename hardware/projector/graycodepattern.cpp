#include "graycodepattern.h"

GrayCodePattern::GrayCodePattern(uint bit, bool inverted):
    bit(bit),
    inverted(inverted)
{
}

QImage GrayCodePattern::generatePattern(int width, int height)
{
    QImage out(width,height,QImage::Format_ARGB32);
    unsigned int values[width];
    unsigned int x,y,gray;
    unsigned int mask = 1<<bit;

    for (x = 0; x < width; x++) {
        gray = binaryToGray(x);
        if(!inverted)
            values[x] = (gray & mask) ? 0:255;
        else
            values[x] = (gray & mask) ? 255:0;
    }

    for (y = 0; y < height; y++) {
            QRgb *destrow = (QRgb*)out.scanLine(y);
            for (x = 0; x < width; ++x) {
                destrow[x] = qRgba(values[x],values[x],values[x],255);
            }
    }

    return out;
}

unsigned int GrayCodePattern::getPatternID()
{
    return (((unsigned int)bit)<<1) |
            (inverted?1U:0U) |
            1U << 12;
}

int GrayCodePattern::binaryToGray(int num)
{
    return (num >> 1) ^ num;
}

unsigned int GrayCodePattern::getBit()
{
    return bit;
}

bool GrayCodePattern::isInverted()
{
    return inverted;
}
