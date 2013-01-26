#include "imageview.h"

ImageView::ImageView(QWidget *parent) :
    QGraphicsView(parent)
{
}

void ImageView::displayImage(cv::Mat_<double> image)
{
    double scale = 255.0;

    QImage dest(image.cols, image.rows, QImage::Format_ARGB32);
    for (int y = 0; y < image.rows; ++y) {
            const double *srcrow = image[y];
            QRgb *destrow = (QRgb*)dest.scanLine(y);
            for (int x = 0; x < image.cols; ++x) {
                    unsigned int color = (unsigned int)(srcrow[x] * scale);
                    if(color>0) color = 255-color;
                    destrow[x] = qRgba(color, color, color, 255);
            }
    }
    return dest;
}
