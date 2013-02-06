#include "changeresolutiondialog.h"
#include "ui_changeresolutiondialog.h"

ChangeResolutionDialog::ChangeResolutionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangeResolutionDialog)
{
    ui->setupUi(this);
}

ChangeResolutionDialog::~ChangeResolutionDialog()
{
    delete ui;
}

void ChangeResolutionDialog::setCamera(Camera *cam)
{
    camera = cam;
}

void ChangeResolutionDialog::accept()
{
    camera->setResolution(ui->resU->value(),ui->resV->value());
    emit resolutionChanged(camera->getResolutionU(),camera->getResolutionV());
    QDialog::accept();
}

void ChangeResolutionDialog::reject()
{
    QDialog::reject();
}

void ChangeResolutionDialog::resolutionChangedU(int u)
{
    camera->setResolution(ui->resU->value(),ui->resV->value());
    emit resolutionChanged(camera->getResolutionU(),camera->getResolutionV());
}

void ChangeResolutionDialog::resolutionChangedV(int v)
{
    camera->setResolution(ui->resU->value(),ui->resV->value());
    emit resolutionChanged(camera->getResolutionU(),camera->getResolutionV());
}
