#include "plotwidget.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <opencv2/imgproc/imgproc.hpp>

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
    cv::Mat col,dCol;
    for(i=0;i<input.size();i++){
        col = input.at(i);
        col = col.reshape(1);
        dCol = data.col(i);
        col.col(0).copyTo(dCol);
    }

    std::cout << "setData(" << data << ")\n";
}

void PlotWidget::plotLines()
{
    QString series,datatable;
    int i,u,v;
    fixHeaders();
    for(i=1;i<headerList.size();i++){
        if(i>1) series.append(",\n     ");
        series.append(tr("'-' using %1 with lines title '%2'")
                      .arg(i).arg(headerList.at(i)));
    }
    for(v=0;v<data.rows;v++){
        for(u=0;u<data.cols;u++){
            datatable.append(tr("  %1").arg(data.at<double>(v,u)));
        }
        datatable.append('\n');
    }
    std::cout << data << '\n';
    QString command = tr("gnuplot << EOF\n"
                         "set terminal png\n"
                         "set output \"plot.png\"\n"
                         "plot %1\n%2\nEOF\n")
            .arg(series).arg(datatable);
//    std::cout << "=====COMMAND=====\n" << command.toLocal8Bit().data()
//              << '\n';
    system(command.toLocal8Bit().data());

}

void PlotWidget::fixHeaders()
{
    int oldSize = headerList.size();
    int i;
    if(oldSize<data.cols){
        for(i=oldSize;i<data.cols;i++){
            headerList.push_back(tr("Series %1").arg(i));
        }
    }
}
