#include "binaryimageprocessor.h"

BinaryImageProcessor::BinaryImageProcessor(QObject *parent) :
    ImageProcessor(parent)
{
}

void BinaryImageProcessor::setHighBit(uint h)
{
    hiBit = h;
}

uint BinaryImageProcessor::getHighBit()
{
    return hiBit;
}

void BinaryImageProcessor::setLowBit(uint l)
{
    loBit = l;
}

uint BinaryImageProcessor::getLowBit()
{
    return loBit;
}

std::vector<ImageDescriptor> BinaryImageProcessor::getRequiredFrames()
{

}

QProjector::Pattern *BinaryImageProcessor::getPattern(ImageDescriptor desc)
{
    QProjector::Pattern *pattern = 0;
    // code for binary pattern is 1; anything else and
    // we're barking up the wrong tree
    if(desc.getInfoFromID(12,4)!=1) return pattern;

    uint bit = desc.getInfoFromID(1,7);
    bool inverted = desc.getInfoFromID(0,1) == 1? true:false;

}
