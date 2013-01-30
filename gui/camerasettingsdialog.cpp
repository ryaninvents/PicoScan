#include "camerasettingsdialog.h"
#include "ui_camerasettingsdialog.h"

#include "../hardware/camera/opencvcamera.h"

CameraSettingsDialog::CameraSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CameraSettingsDialog)
{
    ui->setupUi(this);
    enableControls(false);
    hardware = new HardwareManager;
    manager = new ScanManager;
}

Camera *CameraSettingsDialog::getCurrentCamera()
{
    return hardware->getCamera(idx);
}

CameraSettingsDialog::~CameraSettingsDialog()
{
    delete ui;
}

void CameraSettingsDialog::selectedCameraChanged(int i)
{
    if(i<0) return;
    camera = hardware->getCamera(i);
    idx = i;
    loadCameraDetails();
}

void CameraSettingsDialog::reloadCameras()
{
    hardware->refreshCameras();
    updateCameraList();

    ui->listWidget->setCurrentRow(0);
    selectedCameraChanged(0);
    enableControls(true);
}

void CameraSettingsDialog::renameCamera(QString newName)
{
    getCurrentCamera()->setName(newName);
    ui->listWidget->item(idx)->setText(newName);
}

void CameraSettingsDialog::setLeft()
{
}

void CameraSettingsDialog::setRight()
{
}



void CameraSettingsDialog::enableControls(bool b)
{
    ui->camName->setEnabled(b);
    ui->cellSize->setEnabled(b);
    ui->resolution->setEnabled(b);
    ui->liveFeed->setEnabled(b);
    ui->setLeft->setEnabled(b);
    ui->setRight->setEnabled(b);
    if(!b) ui->liveFeed->setChecked(false);
}

void CameraSettingsDialog::loadCameraDetails()
{
    ui->camName->setText(camera->getName());
    ui->preview->setCamera(camera);
}

void CameraSettingsDialog::updateCameraList()
{
    unsigned int i;
    ui->listWidget->clear();
    for(i=0;i<hardware->numCameras();i++){
        ui->listWidget->addItem(
            hardware->getCamera(i)->getName());
    }
}
