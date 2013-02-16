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

std::vector BinaryImageProcessor::getRequiredFrames()
{

}
