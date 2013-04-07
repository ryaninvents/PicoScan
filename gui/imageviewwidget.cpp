#include "imageviewwidget.h"

ImageViewWidget::ImageViewWidget(QWidget *parent) :
    QGraphicsView(parent),
    hq(false)
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

    origW = image.cols;
    origH = image.rows;
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
        dest = dest.scaled(size(),Qt::KeepAspectRatio,
                           hq? Qt::SmoothTransformation : Qt::FastTransformation);
    }

    displayImage(dest);
}

void ImageViewWidget::displayImage(cv::Mat3b image, bool scale)
{
    origW = image.cols;
    origH = image.rows;
    QImage dest(image.cols, image.rows, QImage::Format_ARGB32);
    for (int y = 0; y < image.rows; ++y) {
            const cv::Vec3b *srcrow = image[y];
            QRgb *destrow = (QRgb*)dest.scanLine(y);
            for (int x = 0; x < image.cols; ++x) {
                    destrow[x] = qRgba(srcrow[x][2], srcrow[x][1], srcrow[x][0], 255);
            }
    }


    if(scale){
        dest = dest.scaled(size(),Qt::KeepAspectRatio,
                           hq? Qt::SmoothTransformation : Qt::FastTransformation);
    }


    displayImage(dest);
}

void ImageViewWidget::displayImage(QImage image)
{
    im = image;
    updateView();
}

void ImageViewWidget::setCrosshairEnabled(bool enabled)
{
    showXhair = enabled;
    updateView();
}

void ImageViewWidget::setHighQualityScale(bool highQuality)
{
    hq = highQuality;
    updateView();
}

void ImageViewWidget::setCrosshair(int x, int y)
{
    setCrosshairEnabled(true);
    crosshair = QPoint(x,y);
}

void ImageViewWidget::updateView()
{

    QGraphicsScene *scene = new QGraphicsScene;
    scene->addPixmap(QPixmap::fromImage(im));
    if(showXhair&&false){

        QPoint xhair = imageToCompCoord(this->crosshair);
        QPen xPen(QColor(255,255,255));
        xPen.setWidth(3);
        xPen.setCapStyle(Qt::FlatCap);
        scene->addLine(xhair.x()-15,xhair.y(),xhair.x()-1,xhair.y(),xPen);
        scene->addLine(xhair.x(),xhair.y()-15,xhair.x(),xhair.y()-1,xPen);
        scene->addLine(xhair.x()+1,xhair.y(),xhair.x()+15,xhair.y(),xPen);
        scene->addLine(xhair.x(),xhair.y()+1,xhair.x(),xhair.y()+15,xPen);
    }
    setScene(scene);
    repaint();
}

QPoint ImageViewWidget::compToImageCoord(QPoint C)
{
    double photoRatio = origW/origH;
    double componentRatio = width()/height();
    QPoint out;
    if(componentRatio>photoRatio){
        out.setY(C.y()*origH/height());
        out.setX(C.x()*origH/height()-(width()-origW)/2);
        if(out.x()<0) out.setX(0);
        if(out.x()<=origW) out.setX(origW-1);
    }else{
        out.setX(C.x()*origW/width());
        out.setY(C.y()*origW/width()-(height()-origH)/2);
        if(out.y()<0) out.setY(0);
        if(out.y()<=origH) out.setY(origH-1);
    }
    return out;
}

QPoint ImageViewWidget::imageToCompCoord(QPoint I)
{
    if(origH==0 || height()==0) return QPoint();
    double photoRatio = origW/origH;
    double componentRatio = width()/height();
    double scale = (photoRatio>componentRatio?origW/width():origH/height());
    QPoint out(((I.x()-origW/2)*scale)+width()/2,
               ((I.y()-origH/2)*scale)+height()/2);
    return out;
}

