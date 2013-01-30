#include "calibrator.h"

#include "hardware/standards/calibrationstandard.h"
#include <opencv2/calib3d/calib3d.hpp>

Calibrator::Calibrator(CalibrationInProgress *dialog, ScanManager *m):
    progressDialog(dialog),
    manager(m)
{

}

void Calibrator::clearFrames()
{
    imagesPointsLeft.clear();
    imagesPointsRight.clear();
}

bool Calibrator::addImagePair(cv::Mat left, cv::Mat right)
{
    CalibrationStandard *std = manager->getStandard();
    bool *success;
    std::vector<cv::Point2d> leftPts, rightPts;

    leftPts = std->getImagePoints(left,success);
    if(!success) return false;

    rightPts = std->getImagePoints(right,success);
    if(!success) return false;

    if(imagePointsLeft.size()==0){
        leftSize = cv::Size(left.cols,left.rows);
        rightSize = cv::Size(right.cols,right.rows);
    }

    imagePointsLeft.push_back(leftPts);
    imagePointsRight.push_back(rightPts);
    return true;
}

bool Calibrator::runCalib()
{
    double rpeLeft, rpeRight;
    CalibrationStandard *std;


}

