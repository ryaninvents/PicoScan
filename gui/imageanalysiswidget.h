#ifndef IMAGEANALYSISWIDGET_H
#define IMAGEANALYSISWIDGET_H

#include <QWidget>
#include <opencv2/core/core.hpp>
#include <opencv2/core/core.hpp>

#define IA_DISPLAY_BINARY 0
#define IA_DISPLAY_REDUCED 1
#define IA_DISPLAY_PHASE 2
#define IA_DISPLAY_HYBRID 3

namespace Ui {
class ImageAnalysisWidget;
}

class ImageAnalysisWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ImageAnalysisWidget(QWidget *parent = 0);
    ~ImageAnalysisWidget();

    void setBinary(cv::Mat b);
    void setReducedBinary(cv::Mat rb);
    void setPhaseMap(cv::Mat p);
    void setHybrid(cv::Mat h);
    void updateImage();

public slots:
    void setImageDisplayed(int n);
    void cropTopChanged(int n);
    void cropBottomChanged(int n);
    void cropLeftChanged(int n);
    void cropRightChanged(int n);
    
private:
    Ui::ImageAnalysisWidget *ui;
    cv::Mat binary;
    int binaryScale;
    cv::Mat reduced;
    int bottomBits;
    cv::Mat phase;
    cv::Mat hybrid;
    int currentDisplayMode;

    int cursorX;
    int cursorY;

    cv::Rect_<int> crop;
};

#endif // IMAGEANALYSISWIDGET_H
