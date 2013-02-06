#include "camerasettingsdialog.h"
#include "ui_camerasettingsdialog.h"

#include "../hardware/camera/opencvcamera.h"

CameraSettingsDialog::CameraSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CameraSettingsDialog)
{
    ui->setupUi(this);
}

CameraSettingsDialog::~CameraSettingsDialog()
{
    delete ui;
}

void CameraSettingsDialog::enableStereo(bool b)
{
    ui->labelSecondCam->setEnabled(b);
    ui->labelSecondRes->setEnabled(b);
    ui->secondDevice->setEnabled(b);
    ui->secondRes->setEnabled(b);
    manager->setStereo(b);
}

void CameraSettingsDialog::setFirstCamera(int idx)
{

}

void CameraSettingsDialog::setSecondCamera(int idx)
{
}

void CameraSettingsDialog::setFirstResolution()
{
}

void CameraSettingsDialog::setSecondResolution()
{
}
