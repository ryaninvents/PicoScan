#include "deviceparametersdialog.h"
#include "ui_deviceparametersdialog.h"

DeviceParametersDialog::DeviceParametersDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeviceParametersDialog)
{
    ui->setupUi(this);
}

DeviceParametersDialog::~DeviceParametersDialog()
{
    delete ui;
}
