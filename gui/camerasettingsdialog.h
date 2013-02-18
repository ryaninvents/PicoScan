#ifndef CAMERASETTINGSDIALOG_H
#define CAMERASETTINGSDIALOG_H

#include <QDialog>
#include "changeresolutiondialog.h"

namespace Ui {
class CameraSettingsDialog;
}

/// Dialog for adding cameras to the setup.
class CameraSettingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    /// Create a new settings dialog.
    explicit CameraSettingsDialog(QWidget *parent = 0);

    /// Destroy this dialog.
    ~CameraSettingsDialog();

signals:
    /// emit debug information
    void debug(QString);

    /// The user has accepted overall camera settings
    void acceptedCameras(QCamera*,QCamera*);

public slots:
    /** Enable or disable stereo. */
    void enableStereo(bool b);

    /// Set the resolution of the first camera
    void setFirstResolution();

    /// Set the resolution of the second camera
    void setSecondResolution();

    /// The user has changed the resolution on camera 1
    void firstResolutionChanged(int u, int v);

    /// The user has changed the resolution on camera 2
    void secondResolutionChanged(int u, int v);

    /// The first camera's mode has changed
    void firstModeChanged(int m);

    /// The second camera's mode has changed
    void secondModeChanged(int m);

    /// The user has accepted settings for the first camera
    void firstCameraSettingsChanged(QCamera *cam);

    /// The user has accepted settings for the second camera
    void secondCameraSettingsChanged(QCamera *cam);

    /// The user has accepted the camera settings
    void accept();
    
private:
    /// UI
    Ui::CameraSettingsDialog *ui;

    /// Resolution dialog for first camera
    ChangeResolutionDialog *firstResDialog;

    /// Resolution dialog for second camera
    ChangeResolutionDialog *secondResDialog;

    /// Settings dialog for first camera
    QDialog *firstSettingsDialog;

    /// Settings dialog for second camera
    QDialog *secondSettingsDialog;

    /// First camera
    QCamera *firstCamera;

    /// Second camera
    QCamera *secondCamera;


};

#endif // CAMERASETTINGSDIALOG_H
