#ifndef BINARYIMAGEPROCESSOR_H
#define BINARYIMAGEPROCESSOR_H

#include "imageprocessor.h"

/// Processes binary images.
class BinaryImageProcessor : public ImageProcessor
{
    Q_OBJECT
public:
    /// Create a new BinaryProcessor.
    explicit BinaryImageProcessor(QObject *parent = 0);

    /// Set the highest bit used in the pattern.
    void setHighBit(uint h);
    /// Get the highest bit used in the pattern.
    uint getHighBit();
    /// Set the lowest bit used in the pattern.
    void setLowBit(uint l);
    /// Get the lowest bit used in the pattern.
    uint getLowBit();
    /// Get a list of ImageDescriptors representing
    /// the frames needed for binary processing.
    std::vector<ImageDescriptor> getRequiredFrames();
    
signals:
    
public slots:

private:
    /// highest bit in pattern
    uint hiBit;
    /// lowest bit in pattern
    uint loBit;
    
};

#endif // BINARYIMAGEPROCESSOR_H
