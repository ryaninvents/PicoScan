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
    /// Notify listeners that a pattern has been projected.
    void patternProjected(QProjector::Pattern*,QProjector*);

public slots:

    /// Project the next pattern in the queue.
    /// \bug May emit patternProjected() before
    /// pattern is actually projected, depending
    /// on the implementation. Might want to move
    /// that to child processes.
    void processQueue();

    /// Tell the projector to hold up, we're doing
    /// something else.
    void waitFor(QObject *holdup);

    /// Tell the projector we're done with whatever
    /// we were doing and move on.
    void stopWaitingFor(QObject *holdup);

protected:

    /// Project a pattern.
    virtual void projectPattern(QProjector::Pattern *)=0;

private:

    /// Queue of patterns waiting to be projected.
    std::vector<QProjector::Pattern*> patternQueue;

    /// Stuff we're waiting for
    std::vector<QObject*> dependencies;

};

#endif // QPROJECTOR_H
