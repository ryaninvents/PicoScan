#include "camerasettingsdialog.h"
#include "ui_camerasettingsdialog.h"

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
