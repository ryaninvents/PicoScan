#include "calibrationstandarddialog.h"
#include "ui_calibrationstandarddialog.h"

#include "../hardware/standards/chessboardstandard.h"
#include "../hardware/standards/dotmatrixstandard.h"

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
    // dot matrix
    case 1:
        s = new DotMatrixStandard(cv::Size(
                                      ui->rows->value(),
                                      ui->cols->value()),
                                  ui->colSpace->value(),
                                  ui->rowSpace->value(),
                                  0);
        break;
    // staggered dot matrix
    case 2:
        s = new DotMatrixStandard(cv::Size(
                                      ui->rows->value(),
                                      ui->cols->value()),
                                  ui->colSpace->value(),
                                  ui->rowSpace->value(),
                                  ui->staggerOffset->value());
        break;
    // if nothing else just use a chessboard
    default:
        s = new ChessboardStandard(cv::Size(
                                       ui->rows->value(),
                                       ui->cols->value()
                                       ),
                                   ui->colSpace->value(),
                                   ui->rowSpace->value());
        break;
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
