#ifndef QPROJECTOR_H
#define QPROJECTOR_H

#include <map>

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
        virtual QImage generatePattern(int width,
                                       int height)=0;
        /// Get an ID that uniquely identifies this pattern's
        /// configuration. This uniquely identifies the pattern
        /// within the queue, so it should be a determinate
        /// function of the pattern's class and parameters.
        virtual unsigned int getPatternID()=0;
    };

    /// Queue up a pattern.
    void queue(QProjector::Pattern *p);

    /// Check to see if we're waiting for something.
    bool checkDependencies();

signals:
    /// Signal to determine whether the projector can
    /// move on or not.
    /// When you have a QObject that needs the projector
    /// to wait for it, use \code
    /// connect(object, SIGNAL(wait()),
    /// projector,SLOT(hold()) \endcode
    /// to cause the projector to wait. When the object
    /// has finished, <tt>disconnect</tt> it.
    void hold();
    /// Notify listeners that a pattern has been projected.
    void patternProjected(QProjector::Pattern*,QProjector*);

public slots:

    /// Project the next pattern in the queue.
    /// \bug May emit patternProjected() before
    /// pattern is actually projected, depending
    /// on the implementation. Might want to move
    /// that to child processes.
    void processQueue();

    /// Wait for a QObject.
    void waitFor(QObject *obj, const char* slot);

    /// Stop waiting for a QObject.
    void stopWaitingFor(QObject *obj);

protected:

    /// Project a pattern.
    virtual void projectPattern(QProjector::Pattern *)=0;

    /// Advance the pattern on disconnect.
    void disconnectNotify(const char *signal);

private:

    /// Queue of patterns waiting to be projected.
    std::vector<QProjector::Pattern*> patternQueue;

};

#endif // QPROJECTOR_H
