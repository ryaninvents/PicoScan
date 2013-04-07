#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QGraphicsView>
#include <opencv2/core/core.hpp>

/// A simple widget for displaying images.
class ImageViewWidget : public QGraphicsView
{
    Q_OBJECT
public:

    /// Create the image viewer.
    explicit ImageViewWidget(QWidget *parent = 0);

    /// Display an OpenCV matrix.
    void displayMat(cv::Mat frame,bool scale);

    /// Display an image.
    /// \param image the image to display
    /// \param scale if \b true, the image will be scaled
    /// to fit within the widget; if \b false, the widget
    /// will display as much as possible
    void displayImage(cv::Mat_<double> image, bool scale);

    /// Display an image, scaled to fit the widget.
    /// \param image the image to display
    void displayImage(cv::Mat_<double> image){displayImage(image,true);}

    /// Display an image.
    /// \param image the image to display
    /// \param scale if \b true, the image will be scaled
    /// to fit within the widget; if \b false, the widget
    /// will display as much as possible
    void displayImage(cv::Mat3b image, bool scale);

    /// Display an image, scaled to fit the widget.
    /// \param image the image to display
    void displayImage(cv::Mat3b image){displayImage(image,true);}

    /// Display the image in the widget without scaling.
    void displayImage(QImage image);



signals:
    void crosshairSet(int,int);


public slots:

    void setCrosshairEnabled(bool enabled);
    void setHighQualityScale(bool highQuality);
    void setCrosshair(int x, int y);

private:
    void updateView();
    bool showXhair;
    QPoint crosshair;
    int origW, origH;
    bool hq;
    QImage im;
    QPoint compToImageCoord(QPoint C);
    QPoint imageToCompCoord(QPoint I);

};

#endif // IMAGEVIEW_H
