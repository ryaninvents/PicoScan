#include "povrayprojectordialog.h"
#include "ui_povrayprojectordialog.h"

PovRayProjectorDialog::PovRayProjectorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PovRayProjectorDialog),
    projector(new PovRayProjector)
{
    ui->setupUi(this);
}

PovRayProjectorDialog::~PovRayProjectorDialog()
{
    delete ui;
}

void PovRayProjectorDialog::accept()
{
    emit projectorUpdated(projector);
    QDialog::accept();
}
