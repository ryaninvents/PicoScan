#ifndef CALIBRATIONDIALOG_H
#define CALIBRATIONDIALOG_H

#include <QDialog>
#include "../hardware/scanmanager.h"

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
    
private:
    Ui::CalibrationDialog *ui;
    ScanManager *manager;
};

#endif // CALIBRATIONDIALOG_H
