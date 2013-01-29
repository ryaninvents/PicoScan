#ifndef CALIBRATIONINPROGRESS_H
#define CALIBRATIONINPROGRESS_H

#include <QDialog>

namespace Ui {
class CalibrationInProgress;
}

class CalibrationInProgress : public QDialog
{
    Q_OBJECT
    
public:
    explicit CalibrationInProgress(QWidget *parent = 0);
    ~CalibrationInProgress();
    
private:
    Ui::CalibrationInProgress *ui;
};

#endif // CALIBRATIONINPROGRESS_H
