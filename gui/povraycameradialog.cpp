#include "povraycameradialog.h"
#include "ui_povraycameradialog.h"

#include <QFileDialog>
#include <QDir>

PovRayCameraDialog::PovRayCameraDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PovRayCameraDialog),
    cam(new PovRayCamera)
{
    ui->setupUi(this);
}

PovRayCameraDialog::~PovRayCameraDialog()
{
    delete ui;
}

void PovRayCameraDialog::locXChanged(double x)
{
    cam->setSimX(x);
}

void PovRayCameraDialog::locYChanged(double y)
{
    cam->setSimY(y);
}

void PovRayCameraDialog::locZChanged(double z)
{
    cam->setSimZ(z);
}

void PovRayCameraDialog::rotAxisXChanged(double x)
{
    rotationAxis[0] = x;
    recalculateCameraRotation();
}

void PovRayCameraDialog::rotAxisYChanged(double y)
{
    rotationAxis[1] = y;
    recalculateCameraRotation();
}

void PovRayCameraDialog::rotAxisZChanged(double z)
{
    rotationAxis[2] = z;
    recalculateCameraRotation();
}

void PovRayCameraDialog::rotationAngleChanged(double angle)
{
    rotationAngle = angle*M_PI/180.0;
    recalculateCameraRotation();
}

void PovRayCameraDialog::focalLengthChanged(double f)
{
    cam->setSimFocalLength(f*1e-3);
}

void PovRayCameraDialog::cellSizeChanged(double cell)
{
    cam->setSimCellSize(cell*1e-6);
}

void PovRayCameraDialog::antialiasingChanged(int aa)
{
    cam->setAntialiasing(aa);
}

void PovRayCameraDialog::changeIniFile()
{
    QString s = QFileDialog::getSaveFileName(
                this,
                tr("Choose INI file location"),
                QDir::home().absolutePath(),
                tr("POV-Ray INI (*.ini)")
                );
    ui->iniFile->setText(s);
    cam->setIniFilename(s);
}

void PovRayCameraDialog::changeSceneFile()
{
    QString s = QFileDialog::getOpenFileName(
                this,
                tr("Choose INI file location"),
                QDir::home().absolutePath(),
                tr("POV-Ray INI (*.ini)")
                );
    ui->sceneFile->setText(s);
    cam->setSceneFilename(s);
}

void PovRayCameraDialog::changeCameraFile()
{
    QString s = QFileDialog::getSaveFileName(
                this,
                tr("Choose camera INC file location"),
                QDir::home().absolutePath(),
                tr("POV-Ray include (*.inc)")
                );
    ui->cameraFile->setText(s);
    cam->setParameterFilename(s);
}

void PovRayCameraDialog::changeRenderFile()
{
    QString s = QFileDialog::getSaveFileName(
                this,
                tr("Choose render output location"),
                QDir::home().absolutePath(),
                tr("PNG image (*.png)")
                );
    ui->renderFile->setText(s);
    cam->setRenderFilename(s);
}

void PovRayCameraDialog::accept()
{
    emit cameraChanged(cam);
    QDialog::accept();
}

void PovRayCameraDialog::recalculateCameraRotation()
{
    double scale = sqrt(rotationAxis.dot(rotationAxis));
    // if the vector has length zero, assume no rotation
    // to avoid a divide-by-zero error
    if(scale<1e-6){
        cam->setSimOrientation(cv::Vec3d());
        return;
    }
    double scaleFactor = rotationAngle/scale;
    cam->setSimOrientation(rotationAxis*scaleFactor);
}
