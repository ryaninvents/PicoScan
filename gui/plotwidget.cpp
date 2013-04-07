#include "plotwidget.h"

#include <stdio.h>
#include <stdlib.h>

PlotWidget::PlotWidget(QWidget *parent) :
    ImageViewWidget(parent)
{
}

void PlotWidget::setHeaders(std::vector<QString> headers)
{
    this->headerList = headers;
}

void PlotWidget::setData(cv::Mat input)
{
    input.convertTo(data,CV_64F);
}

void PlotWidget::setData(std::vector<cv::Mat> input)
{
    int i;
    data = cv::Mat::zeros(1,input.size(),CV_64F);
    cv::Mat col;
    for(i=0;i<input.size();i++){
        col = input.at(i);
        col = col.reshape(1);
        data.col(i) = col;
    }
}

void PlotWidget::plotPoints()
{
    QString series,datatable;
    int i,u,v;
    for(i=1;i<headerList.size();i++){
        if(i>1) series.append(',');
        series.append(tr("'-' using 1:%1 title '%2'")
                      .arg(i).arg(headerList.at(i)));
    }
    for(v=0;v<data.rows;v++){
        for(u=0;u<data.cols;u++){
            datatable.append(tr("%1 ").arg(data.at<double>(v,u)));
        }
        datatable.append('\n');
    }
    QString command = tr("gnuplot << EOF\n"
                         "set terminal png\n"
                         "set output \"plot.png\"\n"
                         "plot %1\n%2\nEOF")
            .arg(series).arg(datatable);
    system(command.toLocal8Bit().data());

}
