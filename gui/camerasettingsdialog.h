#ifndef CAMERASETTINGSDIALOG_H
#define CAMERASETTINGSDIALOG_H

#include <QDialog>
#include "../hardware/camera/camera.h"

#include "../hardware/scanmanager.h"

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
    void setScanManager(ScanManager *m){manager = m;}
    ~CameraSettingsDialog();

public slots:
    void selectedCameraChanged(int i);
    void reloadCameras();
    
private:
    Ui::CameraSettingsDialog *ui;

    /// scan manager
    ScanManager *manager;
    /// currently selected camera
    Camera *camera;
    /// current camera index
    unsigned int idx;

    void enableControls(bool b);

    void loadCameraDetails();
    void updateCameraList();
};

#endif // CAMERASETTINGSDIALOG_H
