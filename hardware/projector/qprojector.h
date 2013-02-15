#ifndef QPROJECTOR_H
#define QPROJECTOR_H

#include "../qopticaldevice.h"
#include "../../gui/imageviewwidget.h"

/// Represents a projector.
class QProjector : public QOpticalDevice
{
    Q_OBJECT
public:
    /// Create a new projector.
    explicit QProjector(QObject *parent = 0);

    /// Represents the actual pattern being projected.
    class Pattern
    {
    public:
        /// Get the pattern.
        virtual QImage generatePattern(int width, int height) = 0;
    };

    /// Convert Gray code to binary.
    static unsigned int grayToBinary(unsigned int num);
    /// Convert binary code to Gray.
    static unsigned int binaryToGray(unsigned int num);

signals:
    /// Notify listeners that a pattern has been projected.
    void patternProjected(uint frameID);

public slots:
    /// Project a pattern.
    
};

#endif // QPROJECTOR_H
