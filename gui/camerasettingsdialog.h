#ifndef CAMERASETTINGSDIALOG_H
#define CAMERASETTINGSDIALOG_H

#include <QDialog>
#include "changeresolutiondialog.h"
#include "../hardware/qscanmananger.h"

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

public slots:
    /** Enable or disable stereo. */
    void enableStereo(bool b);

    /// Set the first camera
    void setFirstCamera(int idx);
    /// Set the second camera
    void setSecondCamera(int idx);
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
    
private:
    /// UI
    Ui::CameraSettingsDialog *ui;

    /// Resolution dialog for first camera
    ChangeResolutionDialog *firstResDialog;
    /// Resolution dialog for second camera
    ChangeResolutionDialog *secondResDialog;

    /// The QScanManager this dialog edits
    QScanManager *manager;

};

#endif // CAMERASETTINGSDIALOG_H
