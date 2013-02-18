#include "opencvcameradialog.h"
#include "ui_opencvcameradialog.h"

OpenCVCameraDialog::OpenCVCameraDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenCVCameraDialog)
{
    ui->setupUi(this);
}

OpenCVCameraDialog::~OpenCVCameraDialog()
{
    delete ui;
}

void OpenCVCameraDialog::accept()
{
    emit cameraAccepted(new QOpenCVCamera(ui->device->value()));
    QDialog::accept();
}
