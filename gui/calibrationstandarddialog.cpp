#include "calibrationstandarddialog.h"
#include "ui_calibrationstandarddialog.h"

#include "../hardware/standards/chessboardstandard.h"

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


CalibrationStandard *CalibrationStandardDialog::getStandard()
{
    CalibrationStandard *s;
    switch(ui->style->currentIndex()){
    default: //only allow chessboard for now
        s = new ChessboardStandard(cv::Size(
                                       ui->rows->value(),
                                       ui->cols->value()
                                       ),
                                   ui->rowSpace->value());
    }
    return s;
}

void CalibrationStandardDialog::standardTypeChanged(int idx)
{
    bool circSize=false, stagger=false;
    switch(idx){
    case 0:
        break;
    case 1:
        circSize = true;
        break;
    case 2:
        circSize = true;
        stagger = true;
        break;
    }

    ui->circleSizeLabel->setEnabled(circSize);
    ui->circleSize->setEnabled(circSize);
    ui->staggerOffsetLabel->setEnabled(stagger);
    ui->staggerOffset->setEnabled(stagger);
}
