#include "imageviewwidget.h"

ImageViewWidget::ImageViewWidget(QWidget *parent) :
    QGraphicsView(parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void ImageViewWidget::displayMat(cv::Mat frame, bool scale)
{
    if(frame.channels()==3){
        cv::Mat3b im;// = frame;
        frame.convertTo(im,CV_8UC3);
        displayImage(im,scale);
    }else{
        cv::Mat_<double> im;// = frame;
        frame.convertTo(im,CV_64F);
        displayImage(im,scale);
    }
}

void ImageViewWidget::displayImage(cv::Mat_<double> image, bool scale)
{
    double inScale = 1.0;

    QImage dest(image.cols, image.rows, QImage::Format_ARGB32);
    for (int y = 0; y < image.rows; ++y) {
            const double *srcrow = image[y];
            QRgb *destrow = (QRgb*)dest.scanLine(y);
            for (int x = 0; x < image.cols; ++x) {
                    int color = 255u-(unsigned int)(srcrow[x] * inScale);
                    uint ucolor;
                    if(color<0) ucolor = 0;
                    else if(color>255) ucolor = 255;
                    else ucolor = 255-color;
                    destrow[x] = qRgba(ucolor, ucolor, ucolor, 255);
            }
    }

    if(scale){
        dest = dest.scaled(size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    }

    displayImage(dest);
}

void ImageViewWidget::displayImage(cv::Mat3b image, bool scale)
{

    QImage dest(image.cols, image.rows, QImage::Format_ARGB32);
    for (int y = 0; y < image.rows; ++y) {
            const cv::Vec3b *srcrow = image[y];
            QRgb *destrow = (QRgb*)dest.scanLine(y);
            for (int x = 0; x < image.cols; ++x) {
                    destrow[x] = qRgba(srcrow[x][2], srcrow[x][1], srcrow[x][0], 255);
            }
    }


    if(scale){
        dest = dest.scaled(size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    }


    displayImage(dest);
}

void ImageViewWidget::displayImage(QImage image)
{
    QGraphicsScene *scene = new QGraphicsScene;
    scene->addPixmap(QPixmap::fromImage(image));

    setScene(scene);
    repaint();
}

