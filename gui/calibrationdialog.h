#ifndef CALIBRATIONDIALOG_H
#define CALIBRATIONDIALOG_H

#include <QDialog>
#include "../hardware/qscanmananger.h"
#include "../geom/calibrator.h"
#include "../gui/calibrationinprogress.h"

namespace Ui {
class CalibrationDialog;
}
/// Two-pane calibration dialog that allows the user
/// to calibrate a stereo pair.
class CalibrationDialog : public QDialog
{
    Q_OBJECT
    
public:
    /// Create the dialog
    explicit CalibrationDialog(QWidget *parent = 0);
    /// Set the scan manager
    void setManager(QScanManager *m);
    /// Get the scan manager
    QScanManager *getManager(){return manager;}
    /// Destroy the dialog
    ~CalibrationDialog();

public slots:
    /// Take a snapshot
    void takeSnap();
    /// Run the calibration algorithm
    void calibrate();
    /// Reset; remove all frames and start over.
    void reset();
    /// Close the dialog.
    void close();
    /// Calibrate the projector.
    void calibrateProjector();
    
private:
    /// UI
    Ui::CalibrationDialog *ui;
    /// manager
    QScanManager *manager;
    /// calibrator
    Calibrator *calibrator;
    /// dialog to display progress to the user
    CalibrationInProgress *inProgress;
};

#endif // CALIBRATIONDIALOG_H
