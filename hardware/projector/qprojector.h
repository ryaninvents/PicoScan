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

signals:
    /// Notify listeners that a pattern has been projected.
    void patternProjected(uint frameID);

public slots:
    /// Project a pattern.
    virtual void projectPattern(QProjector::Pattern *p) = 0;
};

#endif // QPROJECTOR_H
