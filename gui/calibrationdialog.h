#ifndef CALIBRATIONDIALOG_H
#define CALIBRATIONDIALOG_H

#include <QDialog>
#include "../hardware/scanmanager.h"
#include "../geom/calibrator.h"
#include "../gui/calibrationinprogress.h"

namespace Ui {
class CalibrationDialog;
}

class CalibrationDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CalibrationDialog(QWidget *parent = 0);

    void setManager(ScanManager *m);
    ScanManager *getManager(){return manager;}

    ~CalibrationDialog();

public slots:
    void takeSnap();
    void calibrate();
    void reset();
    void close();
    void calibrateProjector();
    
private:
    Ui::CalibrationDialog *ui;
    ScanManager *manager;
    Calibrator *calibrator;
    CalibrationInProgress *inProgress;
};

#endif // CALIBRATIONDIALOG_H
