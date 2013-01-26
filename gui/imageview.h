#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QGraphicsView>
#include <opencv2/core/core.hpp>

class ImageView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ImageView(QWidget *parent = 0);
    void displayImage(cv::Mat_<double> image);

signals:
    
public slots:
    
};

#endif // IMAGEVIEW_H
