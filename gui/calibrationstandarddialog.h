#ifndef CALIBRATIONSTANDARDDIALOG_H
#define CALIBRATIONSTANDARDDIALOG_H

#include <QDialog>
#include "../hardware/qscanmananger.h"
#include "../hardware/standards/calibrationstandard.h"

namespace Ui {
class CalibrationStandardDialog;
}
/// Dialog that allows the user to configure the standard
class CalibrationStandardDialog : public QDialog
{
    Q_OBJECT
    
public:
    /// Create the dialog
    explicit CalibrationStandardDialog(QWidget *parent = 0);
    /// Destroy the dialog
    ~CalibrationStandardDialog();
    /// Set the QScanManager
    void setManager(QScanMananger *m){manager = m;}
    /// Get the calibration standard
    CalibrationStandard *getStandard();

signals:
    /// accept changes
    void accept();

public slots:
    /// user has changed the standard class: chessboard/dot/staggered dot
    void standardTypeChanged(int idx);
    
private:
    /// UI
    Ui::CalibrationStandardDialog *ui;
    /// Manager
    QScanMananger *manager;
};

#endif // CALIBRATIONSTANDARDDIALOG_H
