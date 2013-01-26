#include "projectionpattern.h"

ProjectionPattern::ProjectionPattern(unsigned int width, unsigned int height)
{
    if(width<2 && height<2){
        type = PIXEL;
        size = cv::Size(1,1);
    }else if(width<2){
        type = VERTICAL;
        size = cv::Size(1,height);
    }else if(height<2){
        type = HORIZONTAL;
        size = cv::Size(width,1);
    }else{
        type = TWO_DIMENSIONAL;
        size = cv::Size(width,height);
    }
}

ProjectionPattern::ProjectionPattern(unsigned int width)
{
    if(width<2){
        type = PIXEL;
        size = cv::Size(1,1);
    }else{
        type = HORIZONTAL;
        size = cv::Size(width,1);
    }
}
