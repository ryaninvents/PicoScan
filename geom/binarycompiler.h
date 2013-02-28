#ifndef BINARYCOMPILER_H
#define BINARYCOMPILER_H

#include <QObject>
#include "../hardware/camera/qcamera.h"

class BinaryCompiler : public QObject
{
    Q_OBJECT
public:
    explicit BinaryCompiler(QCamera *cam,
                            QObject *parent = 0);
    void requestFrame(uint nmax);
    
signals:
    void binaryFrameCaptured(cv::Mat,bool);
    void visualBinaryFrame(cv::Mat);
    
public slots:
    void frameCaptured(cv::Mat frame,
                       QCamera *cam,
                       QProjector::Pattern *pattern);

private:
    std::vector<cv::Mat> frames;
    std::vector<bool> captured;
    QCamera *camera;
    bool horiz;
    
    void testAndEmit();

};

#endif // BINARYCOMPILER_H
