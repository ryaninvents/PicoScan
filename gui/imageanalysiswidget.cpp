#include "imageanalysiswidget.h"
#include "ui_imageanalysiswidget.h"
#include "geom/triangulator.h"

ImageAnalysisWidget::ImageAnalysisWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageAnalysisWidget),
    currentDisplayMode(IA_DISPLAY_BINARY),
    crop(0,0,1600,1200),
    binaryScale(1024),
    bottomBits(6)
{
    ui->setupUi(this);
    ui->image->setCrosshairEnabled(true);
}

ImageAnalysisWidget::~ImageAnalysisWidget()
{
    delete ui;
}

void ImageAnalysisWidget::setBinary(cv::Mat b)
{
    binary = b;
    reduced = Triangulator::binaryReduce(b,bottomBits);
    updateImage();
}

void ImageAnalysisWidget::setReducedBinary(cv::Mat rb)
{
    reduced = rb;
}

void ImageAnalysisWidget::setPhaseMap(cv::Mat p)
{
    phase = p;
    updateImage();
}

void ImageAnalysisWidget::setHybrid(cv::Mat h)
{
    hybrid = h;
}

void ImageAnalysisWidget::updateImage()
{
    switch(currentDisplayMode){
    case IA_DISPLAY_BINARY:
        ui->image->displayMat(Triangulator::maphsv(binary(crop),binaryScale),true);
        break;
    case IA_DISPLAY_REDUCED:
        ui->image->displayMat(Triangulator::maphsv(reduced(crop),binaryScale),true);
        break;
    case IA_DISPLAY_PHASE:
        ui->image->displayMat(Triangulator::maphsv(phase(crop),1<<bottomBits),true);
        break;
    }
}

cv::Mat ImageAnalysisWidget::getRowData()
{
    cv::Mat data = -1*cv::Mat::ones(binary.cols,2,CV_64F);
//    binary.row(crossY).reshape(1).copyTo(data.col(0));
//    reduced.row(crossY).reshape(1).copyTo(data.col(1));
    int i;
    for(i=0;i<binary.cols;i++){
        data.at<double>(i,0) = binary.at<double>(crossY,i);
        data.at<double>(i,1) = reduced.at<double>(crossY,i);
    }
    return data;
}

void ImageAnalysisWidget::setImageDisplayed(int n)
{
    currentDisplayMode = n;
    updateImage();
}

void ImageAnalysisWidget::cropTopChanged(int n)
{
    if(n>=ui->cropBot->value()) return;
    crop.y = n;
    crop.height = ui->cropBot->value()-n;
    ui->cropBot->setMinimum(n+1);
    ui->selectedV->setMinimum(n);
    updateImage();
}

void ImageAnalysisWidget::cropBottomChanged(int n)
{
    if(n<=crop.y) return;
    crop.height = n-crop.y;
    ui->cropTop->setMaximum(n-1);
    ui->selectedV->setMaximum(n);
    updateImage();
}

void ImageAnalysisWidget::cropLeftChanged(int n)
{
    if(n>=ui->cropRight->value()) return;
    crop.x = n;
    crop.width = ui->cropRight->value()-n;
    ui->cropRight->setMinimum(n+1);
    ui->selectedU->setMinimum(n);
    updateImage();
}

void ImageAnalysisWidget::cropRightChanged(int n)
{
    if(n<=crop.x) return;
    crop.width = n-crop.x;
    ui->cropLeft->setMaximum(n-1);
    ui->selectedU->setMaximum(n);
    updateImage();
}

void ImageAnalysisWidget::crosshairXChanged(int x)
{
    crossX = x;
    ui->image->setCrosshair(crossX,crossY);
}

void ImageAnalysisWidget::crosshairYChanged(int y)
{
    crossY = y;
    ui->image->setCrosshair(crossX,crossY);
}

void ImageAnalysisWidget::crosshairChanged(int x, int y)
{
    crossX = x;
    crossY = y;
    ui->selectedU->setValue(x);
    ui->selectedV->setValue(y);
}

void ImageAnalysisWidget::requestPlot()
{
    cv::Mat data = getRowData();
    emit plotCrossSection(data);
}
