#include "calibrationstandarddialog.h"
#include "ui_calibrationstandarddialog.h"

CalibrationStandardDialog::CalibrationStandardDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalibrationStandardDialog)
{
    ui->setupUi(this);
}

CalibrationStandardDialog::~CalibrationStandardDialog()
{
    delete ui;
}
