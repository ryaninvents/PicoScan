#include "rotationwidget.h"
#include "ui_rotationwidget.h"

RotationWidget::RotationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RotationWidget)
{
    ui->setupUi(this);
}

RotationWidget::~RotationWidget()
{
    delete ui;
}
