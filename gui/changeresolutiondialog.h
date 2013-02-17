#ifndef CHANGERESOLUTIONDIALOG_H
#define CHANGERESOLUTIONDIALOG_H

#include <QDialog>

#include "../hardware/camera/qcamera.h"

namespace Ui {
class ChangeResolutionDialog;
}
/// Allow the user to set a camera resolution.
class ChangeResolutionDialog : public QDialog
{
    Q_OBJECT
    
public:
    /// Create the dialog.
    explicit ChangeResolutionDialog(QWidget *parent = 0);
    /// Destroy the dialog.
    ~ChangeResolutionDialog();
    /// Set the camera to control.
    void setCamera(QCamera *cam);

public slots:
    /// User has accepted the changes
    void accept();
    /// User has rejected the changes
    void reject();

signals:
    /// User wishes to apply the resolution change
    void resolutionChanged(int u, int v);
    
private:
    /// UI
    Ui::ChangeResolutionDialog *ui;
    /// Camera to modify
    QCamera *camera;
};

#endif // CHANGERESOLUTIONDIALOG_H
