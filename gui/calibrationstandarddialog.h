#ifndef CALIBRATIONSTANDARDDIALOG_H
#define CALIBRATIONSTANDARDDIALOG_H

#include <QDialog>
#include "../hardware/scanmanager.h"
#include "../hardware/standards/calibrationstandard.h"

namespace Ui {
class CalibrationStandardDialog;
}

class CalibrationStandardDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CalibrationStandardDialog(QWidget *parent = 0);
    ~CalibrationStandardDialog();
    void setManager(ScanManager *m){manager = m;}
    CalibrationStandard *getStandard();

signals:
    void accept();

public slots:
    void standardTypeChanged(int idx);
    
private:
    Ui::CalibrationStandardDialog *ui;
    ScanManager *manager;
};

#endif // CALIBRATIONSTANDARDDIALOG_H
