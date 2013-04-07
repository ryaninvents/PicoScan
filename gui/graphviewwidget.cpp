#include "graphviewwidget.h"
#include "ui_graphviewwidget.h"

GraphViewWidget::GraphViewWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphViewWidget)
{
    ui->setupUi(this);
}

GraphViewWidget::~GraphViewWidget()
{
    delete ui;
}
