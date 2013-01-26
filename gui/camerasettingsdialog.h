#ifndef CAMERASETTINGSDIALOG_H
#define CAMERASETTINGSDIALOG_H

#include <QDialog>
#include "../hardware/camera/camera.h"

namespace Ui {
class CameraSettingsDialog;
}

class CameraSettingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CameraSettingsDialog(QWidget *parent = 0);
    ~CameraSettingsDialog();
    
private:
    Ui::CameraSettingsDialog *ui;
    Camera *camera;
};

#endif // CAMERASETTINGSDIALOG_H
