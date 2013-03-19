#ifndef DEVICEPARAMETERSDIALOG_H
#define DEVICEPARAMETERSDIALOG_H

#include <QDialog>

namespace Ui {
class DeviceParametersDialog;
}

class DeviceParametersDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit DeviceParametersDialog(QWidget *parent = 0);
    ~DeviceParametersDialog();
    
private:
    Ui::DeviceParametersDialog *ui;
};

#endif // DEVICEPARAMETERSDIALOG_H
