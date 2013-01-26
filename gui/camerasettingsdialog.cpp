#include "camerasettingsdialog.h"
#include "ui_camerasettingsdialog.h"

#include "../hardware/camera/opencvcamera.h"

CameraSettingsDialog::CameraSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CameraSettingsDialog)
{
    ui->setupUi(this);
    enableControls(false);
}

CameraSettingsDialog::~CameraSettingsDialog()
{
    delete ui;
}

void CameraSettingsDialog::selectedCameraChanged(int i)
{
    camera = manager->getCamera(i);
    idx = i;
    loadCameraDetails();
}

void CameraSettingsDialog::deleteCamera()
{
    manager->removeCamera(idx);
    ui->listWidget->removeItemWidget(
                ui->listWidget->item(idx));
    if(manager->numCameras()==0) enableControls(false);
}

void CameraSettingsDialog::addCamera()
{
    if(manager->addCamera()){
        enableControls(true);
        ui->listWidget->addItem(
                    manager->getCamera(manager->numCameras()-1)->getName());
        selectedCameraChanged(manager->numCameras()-1);
    }
}

void CameraSettingsDialog::enableControls(bool b)
{
    ui->camName->setEnabled(b);
    ui->device->setEnabled(b);
    ui->cellSize->setEnabled(b);
    ui->icon->setEnabled(b);
    ui->capMethod->setEnabled(b);
    ui->resolution->setEnabled(b);
    ui->liveFeed->setEnabled(b);
    if(!b) ui->liveFeed->setChecked(false);
}

void CameraSettingsDialog::loadCameraDetails()
{
    ui->camName->setText(camera->getName());
    ui->preview->setCamera(camera);
}
