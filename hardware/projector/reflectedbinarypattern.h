#ifndef REFLECTEDBINARYPATTERN_H
#define REFLECTEDBINARYPATTERN_H

#include "projectionpattern.h"

class ReflectedBinaryPattern : public ProjectionPattern
{
public:
    ReflectedBinaryPattern(uint size, uint bit, bool inverted);
    PatternType getType(){return HORIZONTAL;}
    static uint binaryToGray(uint num);
    static uint grayToBinary(uint num);
    void setBit(uint b){bit=b;}
    uint getBit(){return bit;}
    void setInverted(bool b){inverted=b;}
    bool isInverted(){return inverted;}
    cv::Mat getPattern();
private:
    uint bit;
    bool inverted;
};

#endif // REFLECTEDBINARYPATTERN_H
