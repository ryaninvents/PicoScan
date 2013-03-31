#ifndef HYBRIDCOMPILER_H
#define HYBRIDCOMPILER_H

#include <QObject>
#include "phasecompiler.h"
#include "binarycompiler.h"

class HybridCompiler : public QObject
{
    Q_OBJECT
public:
    explicit HybridCompiler(QObject *parent = 0);
    
signals:
    
public slots:
    void phaseMapCaptured(cv::Mat frame,bool);
    
private:
    cv::Mat phase;
    cv::Mat encoding;
};

#endif // HYBRIDCOMPILER_H
