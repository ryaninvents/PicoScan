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

void GraphViewWidget::plotLine(QString headers,cv::Mat data)
{
    ui->plot->setHeaders(headers);
    ui->plot->setData(data);
    ui->plot->plotLines();
}

void GraphViewWidget::setXRange(int xMin, int xMax)
{
    ui->plot->setXRange(xMin,xMax);
}
