#include "camerasettingsdialog.h"
#include "ui_camerasettingsdialog.h"

#include "opencvcameradialog.h"

/// \todo make sure everything is initialized to some sort
/// of value on startup
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

    firstModeChanged(0);
    secondModeChanged(0);
}

CameraSettingsDialog::~CameraSettingsDialog()
{
    delete ui;
}

void CameraSettingsDialog::enableStereo(bool b)
{
    ui->labelSecondRes->setEnabled(b);
    ui->labelSecondMode->setEnabled(b);
    ui->secondRes->setEnabled(b);
    ui->secondMode->setEnabled(b);
    ui->secondSettings->setEnabled(b);

}

void CameraSettingsDialog::setFirstResolution()
{
    firstResDialog->setCamera(firstCamera);
    firstResDialog->show();
}

void CameraSettingsDialog::setSecondResolution()
{
    secondResDialog->setCamera(secondCamera);
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

void CameraSettingsDialog::firstModeChanged(int m)
{
    if(ui->firstSettings != 0)
        ui->firstSettings->disconnect();
    switch(m){
    case 0: // OpenCV
    {
        OpenCVCameraDialog *ocv =
                new OpenCVCameraDialog(this);
        ui->firstSettings->setEnabled(true);
        connect(ui->firstSettings,
                SIGNAL(clicked()),
                ocv,
                SLOT(show()));
        connect(ocv,
                SIGNAL(cameraAccepted(QCamera*)),
                this,
                SLOT(firstCameraSettingsChanged(QCamera*)));
        firstSettingsDialog = ocv;
        break;
    }
    case 1: // POV-Ray
    {
        ui->firstSettings->setEnabled(false);
        break;
    }
    case 2: // files
    {
        ui->firstSettings->setEnabled(false);
        break;
    }
    }
}

void CameraSettingsDialog::secondModeChanged(int m)
{
    switch(m){
    case 0: // OpenCV
        ui->secondSettings->setEnabled(false);
        break;
    case 1: // POV-Ray
        ui->secondSettings->setEnabled(false);
        break;
    case 2: // files
        ui->secondSettings->setEnabled(false);
        break;
    }
}

void CameraSettingsDialog::firstCameraSettingsChanged(
        QCamera *cam)
{
    firstCamera = cam;
}

void CameraSettingsDialog::secondCameraSettingsChanged(
        QCamera *cam)
{
}

void CameraSettingsDialog::accept()
{
    emit acceptedCameras(firstCamera,secondCamera);
    QDialog::accept();
}
