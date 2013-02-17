#include "projectorsettingsdialog.h"
#include "ui_projectorsettingsdialog.h"

#include "povrayprojectordialog.h"

ProjectorSettingsDialog::ProjectorSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProjectorSettingsDialog)
{
    ui->setupUi(this);
}

ProjectorSettingsDialog::~ProjectorSettingsDialog()
{
    delete ui;
}

void ProjectorSettingsDialog::projectorTypeChanged(int t)
{
    if(settingsDialog != 0){
        settingsDialog->disconnect();
    }
    switch(t){
    case PROJECTOR_TYPE_MONITOR:
    case PROJECTOR_TYPE_POVRAY:
        PovRayProjectorDialog *d = new PovRayProjectorDialog();
        connect(d,
                SIGNAL(projectorUpdated(QProjector*)),
                this,
                SLOT(updateProjector(QProjector*)));
        settingsDialog = d;
    case PROJECTOR_TYPE_FILE:
    }
}

void ProjectorSettingsDialog::updateProjector(QProjector *p)
{
    projector = p;
}
