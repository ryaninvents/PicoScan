#include "sinusoidpattern.h"

SinusoidPattern::SinusoidPattern(uint width, uint shift, bool horiz):
    period(width),
    shift(shift),
    horiz(horiz),
    brightness(255)
{
}

QImage SinusoidPattern::generatePattern(int width, int height)
{

    if(horiz){
        QImage out(width,height,QImage::Format_ARGB32);
        unsigned int value;
        unsigned int x,y;

        for (y = 0; y < height; y++) {
                QRgb *destrow = (QRgb*)out.scanLine(y);
                value = getSinusoidValue(y);
                for (x = 0; x < width; ++x) {
                    destrow[x] = qRgba(value,value,value,255);
                }
        }

        return out;
    }else{
        QImage out(width,height,QImage::Format_ARGB32);
        unsigned int values[width];
        unsigned int x,y;

        for (x = 0; x < width; x++) {
            values[x] = getSinusoidValue(x);
        }

        for (y = 0; y < height; y++) {
                QRgb *destrow = (QRgb*)out.scanLine(y);
                for (x = 0; x < width; ++x) {
                    destrow[x] = qRgba(values[x],values[x],values[x],255);
                }
        }

        return out;
    }
}

unsigned int SinusoidPattern::getPatternID()
{
    return (((unsigned char)period)<<8) |
            (((unsigned char)shift)) |
            2U << 12;
}

uint SinusoidPattern::getPeriod()
{
    return period;
}

uint SinusoidPattern::getShift()
{
    return shift;
}

void SinusoidPattern::setBrightness(uint b)
{
    brightness = b;
}

uint SinusoidPattern::getSinusoidValue(uint x)
{
    double phase = (double)(x-shift)/(double)period;
    phase = phase*2*M_PI;
    return (uint)((brightness/2-1)*cos(phase)+(brightness/2));
}
