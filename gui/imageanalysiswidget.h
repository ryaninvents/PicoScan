#ifndef IMAGEANALYSISWIDGET_H
#define IMAGEANALYSISWIDGET_H

#include <QWidget>
#include <opencv2/core/core.hpp>
#include <opencv2/core/core.hpp>

#define IA_DISPLAY_BINARY 0
#define IA_DISPLAY_REDUCED 1
#define IA_DISPLAY_PHASE 2
#define IA_DISPLAY_HYBRID 3
#define IA_DISPLAY_ORIGINAL 4
#define IA_DISPLAY_SAMPLE 5

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
    void updateImage();
    cv::Mat getRowData();

public slots:
    void setImageDisplayed(int n);
    void cropTopChanged(int n);
    void cropBottomChanged(int n);
    void cropLeftChanged(int n);
    void cropRightChanged(int n);
    void crosshairXChanged(int x);
    void crosshairYChanged(int y);
    void crosshairChanged(int x,int y);
    void requestPlot();

signals:
    void plotCrossSection(QString, cv::Mat);
    void cropXChanged(int xmin,int xmax);
    
private:
    Ui::ImageAnalysisWidget *ui;
    cv::Mat binary;
    int binaryScale;
    cv::Mat reduced;
    int bottomBits;
    cv::Mat phase;
    cv::Mat hybrid;
    cv::Mat sample;
    int currentDisplayMode;

    int cursorX;
    int cursorY;
    int crossX;
    int crossY;

    cv::Rect_<int> crop;
};

#endif // IMAGEANALYSISWIDGET_H
