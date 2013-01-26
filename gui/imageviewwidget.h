#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QGraphicsView>
#include <opencv2/core/core.hpp>

class ImageViewWidget : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ImageViewWidget(QWidget *parent = 0);
    void displayImage(cv::Mat_<double> image, bool scale);
    void displayImage(cv::Mat_<double> image){displayImage(image,true);}

    void displayImage(cv::Mat3b image, bool scale);
    void displayImage(cv::Mat3b image){displayImage(image,true);}
    void displayImage(QImage image);

signals:
    
public slots:

};

#endif // IMAGEVIEW_H
