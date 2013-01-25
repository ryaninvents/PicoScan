#ifndef CAMERASETTINGSDIALOG_H
#define CAMERASETTINGSDIALOG_H

#include <QDialog>

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
};

#endif // CAMERASETTINGSDIALOG_H
