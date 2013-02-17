#include "camerasettingsdialog.h"
#include "ui_camerasettingsdialog.h"

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
    ui->labelSecondRes->setEnabled(b);
    ui->labelSecondMode->setEnabled(b);
    ui->secondRes->setEnabled(b);
    ui->secondMode->setEnabled(b);
}

void CameraSettingsDialog::setFirstCamera(int idx)
{
    /*
    switch(ui->firstMode->currentIndex()){
    case 0:
//        manager->setFirst(hardware->getCamera(idx));
    default:
//        manager->setFirst(new FileCamera(ui->firstFile->text().toLocal8Bit().data()));
    }*/
}

void CameraSettingsDialog::setSecondCamera(int idx)
{
//    manager->setSecond(hardware->getCamera(idx));
}

void CameraSettingsDialog::setFirstResolution()
{
//    firstResDialog->setCamera(manager->getFirst());
    firstResDialog->show();
}

void CameraSettingsDialog::setSecondResolution()
{
//    secondResDialog->setCamera(manager->getSecond());
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

}

void CameraSettingsDialog::secondModeChanged(int m)
{

}
