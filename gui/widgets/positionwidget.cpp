#include "positionwidget.h"
#include "ui_positionwidget.h"

PositionWidget::PositionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PositionWidget)
{
    ui->setupUi(this);
}

PositionWidget::~PositionWidget()
{
    delete ui;
}
