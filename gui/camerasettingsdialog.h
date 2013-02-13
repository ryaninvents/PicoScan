#ifndef CAMERASETTINGSDIALOG_H
#define CAMERASETTINGSDIALOG_H

#include <QDialog>
#include "../hardware/camera/camera.h"

#include "../hardware/scanmanager.h"
#include "../hardware/hardwaremanager.h"
#include "changeresolutiondialog.h"

namespace Ui {
class CameraSettingsDialog;
}
/**
  Dialog for adding cameras to the setup.
  */
class CameraSettingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CameraSettingsDialog(QWidget *parent = 0);
    /**
      Set the ScanManager that this dialog governs.
      */
    void setHardwareManager(HardwareManager *m){hardware = m;}
    void setScanManager(ScanManager *m){manager = m;}
    ~CameraSettingsDialog();

public slots:
    /** Enable or disable stereo. */
    void enableStereo(bool b);

    void setFirstCamera(int idx);
    void setSecondCamera(int idx);
    void setFirstResolution();
    void setSecondResolution();

    void firstResolutionChanged(int u, int v);
    void secondResolutionChanged(int u, int v);

    void firstModeChanged(int m);
    void secondModeChanged(int m);
    
private:
    Ui::CameraSettingsDialog *ui;

    /// hardware manager
    HardwareManager *hardware;
    /// scan manager
    ScanManager *manager;

    ChangeResolutionDialog *firstResDialog;
    ChangeResolutionDialog *secondResDialog;

};

#endif // CAMERASETTINGSDIALOG_H
