#include "camerasettingsdialog.h"
#include "ui_camerasettingsdialog.h"

#include "../hardware/camera/opencvcamera.h"

CameraSettingsDialog::CameraSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CameraSettingsDialog)
{
    ui->setupUi(this);
    camera = new OpenCVCamera(0);
    ui->preview->setCamera(camera);
}

CameraSettingsDialog::~CameraSettingsDialog()
{
    delete ui;
}
