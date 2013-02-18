#ifndef OPENCVCAMERADIALOG_H
#define OPENCVCAMERADIALOG_H

#include <QDialog>
#include "../hardware/camera/qopencvcamera.h"

namespace Ui {
class OpenCVCameraDialog;
}
/// A dialog for managing cameras that use
/// the OpenCV interface.
class OpenCVCameraDialog : public QDialog
{
    Q_OBJECT
    
public:
    /// Create the dialog
    explicit OpenCVCameraDialog(QWidget *parent = 0);
    /// Destroy the dialog
    ~OpenCVCameraDialog();

signals:
    /// User has decided on a camera
    void cameraAccepted(QCamera *);

public slots:
    /// User has accepted changes
    void accept();
    
private:
    /// UI
    Ui::OpenCVCameraDialog *ui;
};

#endif // OPENCVCAMERADIALOG_H
