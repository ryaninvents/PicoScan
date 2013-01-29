#include "calibrationinprogress.h"
#include "ui_calibrationinprogress.h"

CalibrationInProgress::CalibrationInProgress(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalibrationInProgress)
{
    ui->setupUi(this);
}

CalibrationInProgress::~CalibrationInProgress()
{
    delete ui;
}
