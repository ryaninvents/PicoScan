#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QObject>
#include <vector>
#include "../geom/imagedescriptor.h"

/// Handles processing of different types of images.
/// Create subclasses to handle binary, fringe, hybrid,
/// and other techniques.
class ImageProcessor : public QObject
{
    Q_OBJECT
public:
    /// Create a new ImageProcessor.
    explicit ImageProcessor(QObject *parent = 0);

    /// Get a list of ImageDescriptors representing
    /// the frames that this ImageProcessor needs
    /// in order to generate a coded frame.
    virtual std::vector<ImageDescriptor> getRequiredFrames(){}
    
signals:
    
public slots:
    
};

#endif // IMAGEPROCESSOR_H
