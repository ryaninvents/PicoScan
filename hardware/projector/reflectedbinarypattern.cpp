#include "reflectedbinarypattern.h"

ReflectedBinaryPattern::ReflectedBinaryPattern(uint size,
                                               uint bit,
                                               bool inverted):
    ProjectionPattern(size,0),
    bit(bit),
    inverted(inverted)
{
}

uint ReflectedBinaryPattern::binaryToGray(uint num)
{
    return (num >> 1) ^ num;
}

uint ReflectedBinaryPattern::grayToBinary(uint num)
{
    uint numBits = 8 * sizeof(num);
    uint shift;
    for (shift = 1; shift < numBits; shift = 2 * shift)
    {
        num = num ^ (num >> shift);
    }
    return num;
}

cv::Mat ReflectedBinaryPattern::getPattern()
{
    cv::Mat_<double> m = cv::Mat::zeros(getSize(),CV_64F);
    uint x,gray;
    uint mask = 1<<bit;
    for(x=0;x<getSize().width;x++){
        gray = binaryToGray(x);
        if(inverted)
            m.at<double>(0,x) = (double)(gray&mask?0:1);
        else
            m.at<double>(0,x) = (double)(gray&mask?1:0);
    }
    return m;
}
