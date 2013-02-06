#include "camerasettingsdialog.h"
#include "ui_camerasettingsdialog.h"

#include "../hardware/camera/opencvcamera.h"

CameraSettingsDialog::CameraSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CameraSettingsDialog)
{
    ui->setupUi(this);
    hardware = new HardwareManager;
    manager = new ScanManager;
}

CameraSettingsDialog::~CameraSettingsDialog()
{
    delete ui;
}
