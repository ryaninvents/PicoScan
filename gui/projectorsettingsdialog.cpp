#include "projectorsettingsdialog.h"
#include "ui_projectorsettingsdialog.h"

#include "povrayprojectordialog.h"
#include "secondmonitorprojectordialog.h"

ProjectorSettingsDialog::ProjectorSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProjectorSettingsDialog),
    settingsDialog(0)
{
    ui->setupUi(this);
    projectorTypeChanged(PROJECTOR_TYPE_MONITOR);
}

ProjectorSettingsDialog::~ProjectorSettingsDialog()
{
    delete ui;
}

void ProjectorSettingsDialog::projectorTypeChanged(int t)
{
    if(this->settingsDialog != 0){
        settingsDialog->disconnect();
        ui->typeConfig->disconnect();
    }
    switch(t){
    case PROJECTOR_TYPE_MONITOR:
    {
        SecondMonitorProjectorDialog *d =
                new SecondMonitorProjectorDialog();
        connect(d,
                SIGNAL(projectorUpdated(QProjector*)),
                this,
                SLOT(updateProjector(QProjector*)));
        connect(ui->typeConfig,
                SIGNAL(clicked()),
                d,
                SLOT(show()));
        ui->typeConfig->setEnabled(false);
        break;
    }
    case PROJECTOR_TYPE_POVRAY:
    {
        PovRayProjectorDialog *d = new PovRayProjectorDialog();
        connect(d,
                SIGNAL(projectorUpdated(QProjector*)),
                this,
                SLOT(updateProjector(QProjector*)));
        connect(ui->typeConfig,
                SIGNAL(clicked()),
                d,
                SLOT(show()));
        ui->typeConfig->setEnabled(true);
        settingsDialog = d;
        break;
    }
    case PROJECTOR_TYPE_FILE:
    {
        ui->typeConfig->setEnabled(false);
        break;
    }
    default:
        ui->typeConfig->setEnabled(false);
        break;
    }
}

void ProjectorSettingsDialog::updateProjector(QProjector *p)
{
    projector = p;
}
