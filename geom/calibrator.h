#ifndef CALIBRATOR_H
#define CALIBRATOR_H

#include <QObject>
#include "../hardware/camera/qcamera.h"

class Calibrator : public QObject
{
    Q_OBJECT
public:
    explicit Calibrator(QObject *parent = 0);
    enum CalMode{
        CAMERA_MODE,
        PROJECTOR_MODE
    };
    
signals:
    
public slots:
    void frameCaptured(cv::Mat frame,
                       QCamera *cam,
                       QProjector::Pattern *pattern);

private:
    QCamera *camera1;
    QCamera *camera2;
    std::vector<cv::Mat> frames1;
    std::vector<cv::Mat> frames2;
    
};

#endif // CALIBRATOR_H
