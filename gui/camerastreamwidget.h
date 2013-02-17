#ifndef CAMERASTREAMWIDGET_H
#define CAMERASTREAMWIDGET_H

#include <QTimer>
#include "../hardware/camera/qcamera.h"
#include "imageviewwidget.h"

/**
  A widget capable of streaming camera images to the screen by itself.
  */
class CameraStreamWidget : public ImageViewWidget
{
    Q_OBJECT
public:
    explicit CameraStreamWidget(QWidget *parent = 0);
    /**
      Is this widget currently streaming from a camera?
      \returns \c true if running, \c false if not
      */
    bool isRunning(){return running;}

    /**
      Change the camera attached to the widget.
      \param cam The camera to stream to the widget.
      */
    void setCamera(QCamera *cam){camera = cam;noCamera = false;}

    /**
      Set the frame-taking interval of the widget, in milliseconds.
      */
    void setInterval(unsigned int i){interval = i;}

    /** Set the frame rate of the widget, in frames per second. */
    void setFrameRate(unsigned int i){interval = 1000/i;}

signals:
    
public slots:
    /** Start displaying camera images. */
    void startCameraStream();
    /** Stop displaying camera images. */
    void stopCameraStream();
    /** Turn on or off camera images. */
    void toggleCameraStream(bool b);
    /** Update the widget with a single image. */
    void updateImage();
    /** Close the camera stream so you can destroy the widget. */
    void closeCameraStream();

private:
    /// Timer for controlling streaming
    /// \deprecated streaming will be controlled by camera instead
    QTimer *timer;
    /// The camera to stream from
    QCamera *camera;
    /// whether or not we're live
    bool running;
    /// if we have no camera
    bool noCamera;
    /// the interval to update on
    /// \deprecated see note on \a timer
    unsigned int interval;
    
};

#endif // CAMERASTREAMWIDGET_H
