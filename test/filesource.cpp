#include "filesource.h"

#include <QString>
#include <iostream>

FileSource::FileSource()
{
}

uint FileSource::getIndexFromPattern(FileSource::ImageType ptype, uint param1, uint param2, uint param3)
{
    switch(ptype){
    case GRAY:{
        uint bit = param1;
        uint flip = param2>0?1:0;
        uint which = param3;
        return (bit*avg*2)+(flip*avg)+which;
    }
    case CALIBRATION:{
        return param1;
        break;
    }
    case SINUSOID:{
        uint shift = param1;
        uint which = param3;
        return (shift*avg) + which;
        break;
    }
    }
}

void FileSource::setDirectory(QString str)
{
    fileDirectory = QDir(str);
}

cv::Mat FileSource::getAveragedGrayCodeImage(uint bit, bool flipped)
{
    QDir grayDir(fileDirectory);
    uint idx ;
    grayDir.cd(QString("gray"));
    grayDir.setSorting(QDir::Name);
    QStringList filters;
    filters << "*.jpg";
    grayDir.setNameFilters(filters);
    cv::Mat out,tmp;
    bool outInited = false;
    uint which;
    for(which=0;which<avg;which++){
        idx = (bit*avg*2)+((flipped?1:0)*avg)+which;
        std::cout << "Gray code for bit " << bit << " flip " << flipped
                  << "(" << idx << ")\n";
        if(outInited){
            tmp = loadImageFrom(grayDir,idx);
            std::cout << "Image " << tmp.cols << "x" << tmp.rows
                      << "\n";
            out+=tmp;
        }else{
            out = loadImageFrom(grayDir,idx);
            std::cout << "Image " << out.cols << "x" << out.rows
                      << "\n";
            outInited = true;
        }
    }
    out = out/avg;
    return out;
}

cv::Mat FileSource::getAveragedSinusoid(uint shift)
{
    QDir dir(fileDirectory);
    uint idx ;
    dir.cd(QString("phase"));
    dir.setSorting(QDir::Name);
    cv::Mat out,tmp;
    bool outInited = false;
    uint which;
    for(which=0;which<avg;which++){
        idx = shift*avg+which;
        if(outInited){
            tmp = loadImageFrom(dir,idx);
            out+=tmp;
        }else{
            out = loadImageFrom(dir,idx);
            outInited = true;
        }
    }
    out = out/avg;
    return out;
}

cv::Mat FileSource::getCalibrationImage(uint idx)
{
    cv::Mat out;
    return out;
}

void FileSource::setAveraging(uint averaging)
{
    avg = averaging;
}

cv::Mat FileSource::loadImageFrom(QDir dir, uint idx)
{
    QFileInfoList list = dir.entryInfoList();
    QString path = list.at(idx).absoluteFilePath();
    cv::Mat out = cv::imread(path.toLocal8Bit().data(),CV_LOAD_IMAGE_GRAYSCALE);
    return out;
}
