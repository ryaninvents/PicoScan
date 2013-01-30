#ifndef CAMERASETTINGSDIALOG_H
#define CAMERASETTINGSDIALOG_H

#include <QDialog>
#include "../hardware/camera/camera.h"

#include "../hardware/scanmanager.h"
#include "../hardware/hardwaremanager.h"

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
    Camera *getCurrentCamera();
    ~CameraSettingsDialog();

public slots:
    void selectedCameraChanged(int i);
    void reloadCameras();
    void renameCamera(QString newName);
    void setLeft();
    void setRight();
    
private:
    Ui::CameraSettingsDialog *ui;

    /// hardware manager
    HardwareManager *hardware;
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
