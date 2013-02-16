#ifndef QPROJECTOR_H
#define QPROJECTOR_H

#include "../qopticaldevice.h"
#include "../../gui/imageviewwidget.h"
#include "../../geom/imagedescriptor.h"

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
        virtual QImage generatePattern(int width, int height){}
        /// Get an ID that uniquely identifies this pattern's
        /// configuration. This identifies the pattern among
        /// other patterns in the same sequence, so it need
        /// not encode size; rather, it should include data
        /// that changes over the course of the measurement,
        /// e.g., fringe density or phase shift.
        virtual unsigned int getPatternID(){}
    };

signals:
    /// Notify listeners that a pattern has been projected.
    void patternProjected(ImageDescriptor);

public slots:
    /// Project a pattern.
    virtual void projectPattern(QProjector::Pattern *,
                                ImageDescriptor){}
};

#endif // QPROJECTOR_H
