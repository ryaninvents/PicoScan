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
        void setID(int id);
        int getID();
    private:
        /// An integer representing this pattern.
        /// Use it as you like.
        int _id;
    };

    /// Queue up a pattern.
    void queue(QProjector::Pattern *p);

    /// Check to see if we're waiting for something.
    bool checkDependencies();

    /// Check to see if we have any dependencies.
    bool hasDependencies();

    /// Count dependencies
    uint countDependencies();

    /// Get the current pattern
    QProjector::Pattern *getCurrentPattern();

signals:
    /// Signal to determine whether the projector can
    /// move on or not.
    /// When you have a QObject that needs the projector
    /// to wait for it, use \code
    /// connect(object, SIGNAL(wait()),
    /// projector,SLOT(hold()) \endcode
    /// to cause the projector to wait. When the object
    /// has finished, <tt>disconnect</tt> it.
    void aboutToAdvance();
    /// Notify listeners that a pattern has been projected.
    void patternProjected(QProjector::Pattern*,QProjector*);

public slots:

    /// Received when a camera decides whether or not
    /// it would like the projector to continue.
    void permissionToAdvance(bool canAdvance);

    /// Request an advance.
    void requestAdvance();

protected:

    /// Project a pattern.
    virtual void projectPattern(QProjector::Pattern *)=0;

private:

    /// Queue of patterns waiting to be projected.
    std::vector<QProjector::Pattern*> patternQueue;

    /// Project the next pattern in the queue.
    /// \bug May emit patternProjected() before
    /// pattern is actually projected, depending
    /// on the implementation. Might want to move
    /// that to child processes.
    void processQueue();

    /// How many listeners are we waiting for?
    uint listenersWaitingFor;

    /// How many listeners have denied permission to advance?
    uint listenersDenyingPermission;

    /// What's projecting now?
    QProjector::Pattern *currentPattern;

};

#endif // QPROJECTOR_H
