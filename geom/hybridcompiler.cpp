#include "hybridcompiler.h"

HybridCompiler::HybridCompiler(QObject *parent) :
    QObject(parent)
{
}

void HybridCompiler::phaseMapCaptured(cv::Mat frame, bool)
{
}
