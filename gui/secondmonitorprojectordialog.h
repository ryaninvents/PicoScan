#ifndef SECONDMONITORPROJECTORDIALOG_H
#define SECONDMONITORPROJECTORDIALOG_H

#include <QDialog>

namespace Ui {
class SecondMonitorProjectorDialog;
}

class SecondMonitorProjectorDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SecondMonitorProjectorDialog(QWidget *parent = 0);
    ~SecondMonitorProjectorDialog();
    
private:
    Ui::SecondMonitorProjectorDialog *ui;
};

#endif // SECONDMONITORPROJECTORDIALOG_H
