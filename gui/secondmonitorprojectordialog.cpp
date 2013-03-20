#include "secondmonitorprojectordialog.h"
#include "ui_secondmonitorprojectordialog.h"

SecondMonitorProjectorDialog::SecondMonitorProjectorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecondMonitorProjectorDialog)
{
    ui->setupUi(this);
}

SecondMonitorProjectorDialog::~SecondMonitorProjectorDialog()
{
    delete ui;
}
