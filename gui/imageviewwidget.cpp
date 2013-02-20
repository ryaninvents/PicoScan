#include "imageviewwidget.h"

ImageViewWidget::ImageViewWidget(QWidget *parent) :
    QGraphicsView(parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void ImageViewWidget::displayImage(cv::Mat_<double> image, bool scale)
{
    double inScale = 255.0;

    QImage dest(image.cols, image.rows, QImage::Format_ARGB32);
    for (int y = 0; y < image.rows; ++y) {
            const double *srcrow = image[y];
            QRgb *destrow = (QRgb*)dest.scanLine(y);
            for (int x = 0; x < image.cols; ++x) {
                    unsigned int color = (unsigned int)(srcrow[x] * inScale);
                    if(color<0) color = 0;
                    if(color>0) color = 255-color;
                    destrow[x] = qRgba(color, color, color, 255);
            }
    }

    if(scale){
        dest = dest.scaled(size(),Qt::KeepAspectRatio);
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
        dest = dest.scaled(size(),Qt::KeepAspectRatio);
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

