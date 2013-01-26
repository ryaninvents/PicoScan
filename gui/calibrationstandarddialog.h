#ifndef CALIBRATIONSTANDARDDIALOG_H
#define CALIBRATIONSTANDARDDIALOG_H

#include <QDialog>

namespace Ui {
class CalibrationStandardDialog;
}

class CalibrationStandardDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CalibrationStandardDialog(QWidget *parent = 0);
    ~CalibrationStandardDialog();
    
private:
    Ui::CalibrationStandardDialog *ui;
};

#endif // CALIBRATIONSTANDARDDIALOG_H
