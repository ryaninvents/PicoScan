#ifndef QFILECAMERA_H
#define QFILECAMERA_H

#include "qcamera.h"

/// A "camera" that captures images from files.
class QFileCamera : public QCamera
{
    Q_OBJECT
public:
    /// Create a new QFileCamera with the given filename pattern.
    explicit QFileCamera(QString fnm,QObject *parent = 0);

    /// Is the camera open?
    /// \returns Always \a true, since there's no hardware to manage.
    bool isOpen(){return true;}

    /// Release resources (does nothing).
    void release(){}

    /// Get the current filename.
    QString getFilename(){return fnm;}

    /// Set the filename pattern.
    void setFilename(QString s){fnm=s;}
    
signals:
    
public slots:
    /// Request a frame from file.
    void requestFrame(ImageDescriptor desc);

private:

    /// Filename pattern.
    QString fnm;

    /// Get filename
    char *getFilename(ImageDescriptor desc);
    
};

#endif // QFILECAMERA_H
