#include "imageanalysiswidget.h"
#include "ui_imageanalysiswidget.h"
#include "geom/triangulator.h"

#include <opencv2/highgui/highgui.hpp>

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
    sample = cv::imread("/home/ryan/gradient.png",CV_LOAD_IMAGE_GRAYSCALE);
    sample.convertTo(sample,CV_64F);
    binary = cv::Mat::zeros(1600,1200,CV_64F);
    reduced = cv::Mat::zeros(1600,1200,CV_64F);
    phase = cv::Mat::zeros(1600,1200,CV_64F);
    hybrid = cv::Mat::zeros(1600,1200,CV_64F);
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
    p.convertTo(phase,CV_64F);
    phase = phase * 2;
    hybrid = Triangulator::combineBinaryAndPhase(
                binary,
                phase,
                bottomBits);
    emit hybridMapComputed(hybrid);
//    int u,v;
//    for(v=0;v<hybrid.rows;v++){
//        for(u=0;u<hybrid.cols;u++){
//            if(reduced.at<double>(v,u)<0
//                    || phase.at<double>(v,u) < 0){
//                hybrid.at<double>(v,u) = -1;
//            }
//        }
//    }
    updateImage();
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
    case IA_DISPLAY_HYBRID:
        ui->image->displayMat(Triangulator::maphsv(hybrid(crop),binaryScale),true);
        break;
//    case IA_DISPLAY_SAMPLE:
//        ui->image->displayMat(Triangulator::maphsv(sample(crop),255),true);
//        break;
    }
}

cv::Mat ImageAnalysisWidget::getRowData()
{
    cv::Mat data = -1*cv::Mat::ones(sample.cols,3,CV_64F);
//    binary.row(crossY).reshape(1).copyTo(data.col(0));
//    reduced.row(crossY).reshape(1).copyTo(data.col(1));
    int i,x;
    for(x=crop.x;x<=crop.x+crop.width;x++){
        i = x-crop.x;
        data.at<double>(i,0) = x;
        data.at<double>(i,1) = binary.at<double>(crossY,x);
//        data.at<double>(i,2) = reduced.at<double>(crossY,x);
//        data.at<double>(i,3) = phase.at<double>(crossY,x);
        data.at<double>(i,2) = hybrid.at<double>(crossY,x);
//        data.at<double>(i,2) = sample.at<double>(crossY,i);
//        data.at<double>(i,1) = reduced.at<double>(crossY,i);
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
    emit cropXChanged(crop.x,crop.x+crop.width);
    updateImage();
}

void ImageAnalysisWidget::cropRightChanged(int n)
{
    if(n<=crop.x) return;
    crop.width = n-crop.x;
    ui->cropLeft->setMaximum(n-1);
    ui->selectedU->setMaximum(n);
    emit cropXChanged(crop.x,crop.x+crop.width);
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
    emit plotCrossSection(tr("Position|Binary|Hybrid"),data);
}
