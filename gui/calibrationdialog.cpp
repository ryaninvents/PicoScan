#include "calibrationdialog.h"
#include "ui_calibrationdialog.h"
#include <QMessageBox>

CalibrationDialog::CalibrationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalibrationDialog)
{
    ui->setupUi(this);
}

void CalibrationDialog::setManager(ScanManager *m)
{
    manager = m;
    if(manager->numCameras()<2){
        QMessageBox::warning(this,"Not enough cameras","You must have at least two cameras attached to the system.");
        close();
    }
    ui->previewLeft->setCamera(manager->getCamera(0));
    ui->previewRight->setCamera(manager->getCamera(1));
}

CalibrationDialog::~CalibrationDialog()
{
    delete ui;
}
