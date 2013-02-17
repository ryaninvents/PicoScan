#include "povraycameradialog.h"
#include "ui_povraycameradialog.h"

PovRayCameraDialog::PovRayCameraDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PovRayCameraDialog)
{
    ui->setupUi(this);
}

PovRayCameraDialog::~PovRayCameraDialog()
{
    delete ui;
}
