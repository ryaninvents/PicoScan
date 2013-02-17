#ifndef POVRAYCAMERADIALOG_H
#define POVRAYCAMERADIALOG_H

#include <QDialog>
#include "../hardware/camera/povraycamera.h"

namespace Ui {
class PovRayCameraDialog;
}
/// A dialog for setting up a POV-Ray camera.
class PovRayCameraDialog : public QDialog
{
    Q_OBJECT
    
public:
    /// Create the dialog
    explicit PovRayCameraDialog(QWidget *parent = 0);
    /// Destroy the dialog
    ~PovRayCameraDialog();

signals:
    /// The user has accepted changes.
    void cameraChanged(QCamera *cam);
    
public slots:
    /// user edited location x
    void locXChanged(double x);

    /// user edited location y
    void locYChanged(double y);

    /// user edited location z
    void locZChanged(double z);

    /// user edited rotation axis x
    void rotAxisXChanged(double x);

    /// user edited rotation axis y
    void rotAxisYChanged(double y);

    /// user edited rotation axis z
    void rotAxisZChanged(double z);

    /// user edited rotation angle
    void rotationAngleChanged(double angle);

    /// user edited focal length
    void focalLengthChanged(double f);

    /// user edited cell size
    void cellSizeChanged(double cell);

    /// user edited antialiasing value
    void antialiasingChanged(int aa);

    /// user wants to change the ini file
    void changeIniFile();

    /// user wants to change the scene file
    void changeSceneFile();

    /// user wants to change the camera file
    void changeCameraFile();

    /// user wants to change the render file
    void changeRenderFile();

    /// user has accepted changes
    void accept();


private:

    /// UI
    Ui::PovRayCameraDialog *ui;

    /// The camera
    PovRayCamera *cam;

    /// The rotation axis
    cv::Vec3d rotationAxis;

    /// The rotation angle, in radians
    double rotationAngle;

    /// Recalculate the rotation vector.
    void recalculateCameraRotation();
};

#endif // POVRAYCAMERADIALOG_H
