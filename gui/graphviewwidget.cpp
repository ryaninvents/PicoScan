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

void GraphViewWidget::plotLine(cv::Mat data)
{
    ui->plot->setData(data);
    ui->plot->plotLines();
}
