#ifndef PROJECTORDEPENDENT_H
#define PROJECTORDEPENDENT_H

#include <QObject>

/// Represents a process that depends on a projected frame.
/// Specifically, ProjectorDependent allows a camera to say,
/// "wait, don't change frames yet; I'm not done taking a
/// picture."
class ProjectorDependent : public QObject
{
    Q_OBJECT
public:
    /// Create a new ProjectorDependent.
    explicit ProjectorDependent(QObject *parent = 0);

    /// Set this ProjectorDependent's ID.
    void setID(uint i);

    /// Is this dependency satisfied?
    bool isSatisfied();

    /// Reset this dependency to unsatisfied.
    void reset();
    
signals:
    /// Allow the projector to move on.
    /// Depending on the state of other ProjectorDependents,
    /// this may or may not happen immediately.
    void permitRelease();

    /// Remove this ProjectorDependent from the QProjector's
    /// list.
    void deregister(uint);
    
public slots:
    /// Announce that we're finished with whatever
    /// we were doing and the projector can move on.
    void satisfy();
    /// Deregister this ProjectorDependent from the
    /// QProjector.
    void deregister();
    
private:
    /// ID; also the position in the QProjector's list
    uint dependencyId;

    /// Is this dependency satisfied?
    bool satisfied;
};

#endif // PROJECTORDEPENDENT_H
