#include "plotwidget.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include <opencv2/imgproc/imgproc.hpp>

PlotWidget::PlotWidget(QWidget *parent) :
    ImageViewWidget(parent),
    autoX(true)
{
}

void PlotWidget::setHeaders(std::vector<QString> headers)
{
    this->headerList = headers;
}

void PlotWidget::setHeaders(QString headers)
{
    QStringList list = headers.split(tr("|"));
    headerList.clear();
    for(int i=0;i<list.size();i++){
        headerList.push_back(list.at(i));
    }
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

void PlotWidget::setXRange(double min, double max)
{
    xmin = min;
    xmax = max;
    autoX = false;
}

void PlotWidget::clearXRange()
{
    autoX = true;
}

void PlotWidget::plotLines()
{
    QString series,datatable;
    int i,u,v;
    fixHeaders();
    for(i=1;i<headerList.size();i++){
        if(i>1) series.append(", ");
        series.append(tr(" 'plotdata.dat' using 1:(%1>0?%1:1/0) with lines title '%2'")
                      .arg(QString("\\$").append(QString("%1").arg(i+1))).arg(headerList.at(i)));
    }
    std::ofstream plotdata("plotdata.dat");
    for(v=0;v<data.rows;v++){
        for(u=0;u<data.cols;u++){
            plotdata << data.at<double>(v,u) << "   ";
//            datatable.append(tr("  %1").arg(data.at<double>(v,u)));
        }
//        datatable.append('\n');
        plotdata << '\n';
    }
    plotdata.close();
//    std::cout << data << '\n';
    QString command = tr("gnuplot << EOF\n"
                         "set title \"%5\"\n"
                         "set terminal png enhanced font \"cmr10,16\" size %3,%4\n"
                         "set output \"plot.png\"\n"
                         "plot %6 %1\n%2\nEOF\n")
            .arg(series).arg(datatable).arg(width()-2).arg(height()-2)
            .arg(title).arg(autoX?tr(""):tr("[%1:%2]").arg(xmin).arg(xmax));
    char *cmd = command.toLocal8Bit().data();
    std::cout << cmd << '\n';
    system(cmd);
    QImage plotImg("plot.png");
    displayImage(plotImg);

}

void PlotWidget::setTitle(QString t)
{
    title = t;
    plotLines();
}

void PlotWidget::fixHeaders()
{
    int oldSize = headerList.size();
    int i;
    for(i=oldSize;i<data.cols;i++){
        headerList.push_back(tr("Intensity %1").arg(i));
    }
}
