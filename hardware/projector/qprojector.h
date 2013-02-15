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
    
    /// Represents the GUI element used to project images.
    class Screen : public ImageViewWidget
    {
        Q_OBJECT
    public:
        /// Create a new screen.
        explicit Screen(QWidget *parent = 0);
        /// Set up this screen to project on a particular monitor.
        void projectOnDisplay(int n);
        /// Count the number of displays on the user's system.
        int getDisplayCount();
        /// Project a binary pattern.
        void projectBinary(int bit, bool inverted, int maxBright);
        /// Project a sinusoidal pattern.
        void projectSinusoid(int period, double shift, int maxBright);
        /// Project white.
        void projectWhite();
        /// Convert Gray code to binary.
        static unsigned int grayToBinary(unsigned int num);
        /// Convert binary code to Gray.
        static unsigned int binaryToGray(unsigned int num);
    };

    /// Represents the actual pattern being projected.
    class Pattern
    {
    public:
        /// Get the pattern.
        virtual QImage generatePattern(int width, int height) = 0;
    };

signals:
    /// Notify listeners that a pattern has been projected.
    void patternProjected(uint frameID);

public slots:
    /// Project a pattern.
    void projectPattern();
    
};

#endif // QPROJECTOR_H
