#include "camerasettingsdialog.h"
#include "ui_camerasettingsdialog.h"

#include "../hardware/camera/opencvcamera.h"

CameraSettingsDialog::CameraSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CameraSettingsDialog)
{
    ui->setupUi(this);
    firstResDialog = new ChangeResolutionDialog;
    secondResDialog = new ChangeResolutionDialog;


    connect(firstResDialog,SIGNAL(resolutionChanged(int,int)),
            this,SLOT(firstResolutionChanged(int,int)));

    connect(secondResDialog,SIGNAL(resolutionChanged(int,int)),
            this,SLOT(secondResolutionChanged(int,int)));
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
    manager->setFirst(hardware->getCamera(idx));
}

void CameraSettingsDialog::setSecondCamera(int idx)
{
    manager->setSecond(hardware->getCamera(idx));
}

void CameraSettingsDialog::setFirstResolution()
{
    firstResDialog->setCamera(manager->getFirst());
    firstResDialog->show();
}

void CameraSettingsDialog::setSecondResolution()
{
    secondResDialog->setCamera(manager->getSecond());
    secondResDialog->show();
}

void CameraSettingsDialog::firstResolutionChanged(int u, int v)
{
    QString buttonText = QString("%1 x %2").arg(u).arg(v);
    ui->firstRes->setText(buttonText);
}

void CameraSettingsDialog::secondResolutionChanged(int u, int v)
{
    QString buttonText = QString("%1 x %2").arg(u).arg(v);
    ui->secondRes->setText(buttonText);
}
