#include "fileprojectordialog.h"
#include "ui_fileprojectordialog.h"

FileProjectorDialog::FileProjectorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileProjectorDialog)
{
    ui->setupUi(this);
}

FileProjectorDialog::~FileProjectorDialog()
{
    delete ui;
}
