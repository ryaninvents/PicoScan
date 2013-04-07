#include "plotwidget.h"

PlotWidget::PlotWidget(QWidget *parent) :
    ImageViewWidget(parent)
{
}

void PlotWidget::setHeaders(std::vector<QString> headers)
{
    this->headers = headers;
}

void PlotWidget::setData(cv::Mat input)
{
    input.convertTo(data,CV_64F);
}
