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
    void requestFrame(uint frameID);

    /// Request a grayscale frame from file.
    void requestGrayscaleFrame(uint frameID);

private:

    /// Filename pattern.
    QString fnm;
    
};

#endif // QFILECAMERA_H
