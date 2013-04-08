#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include "imageviewwidget.h"

#include <vector>

class PlotWidget : public ImageViewWidget
{
    Q_OBJECT
public:
    explicit PlotWidget(QWidget *parent = 0);
    void setHeaders(std::vector<QString> headers);
    void setHeaders(QString headers);
    void setData(cv::Mat input);
    void setData(std::vector<cv::Mat> input);
    void setXRange(double min, double max);
    void clearXRange();
    void plotLines();
signals:
    
public slots:
    void setTitle(QString t);

private:
    std::vector<QString> headerList;
    cv::Mat data;

    void fixHeaders();
    QString title;
    int mode;
    double xmin, xmax;
    bool autoX;
    
};

#endif // PLOTWIDGET_H
