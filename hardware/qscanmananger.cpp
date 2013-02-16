#include "qscanmananger.h"

QScanMananger::QScanMananger(QObject *parent) :
    QObject(parent)
{
}

void QScanMananger::frameReturned(cv::Mat frame,
                                  qint64 timestamp,
                                  uint frameID,
                                  uint cameraID){

}
