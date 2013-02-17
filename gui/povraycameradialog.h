#ifndef POVRAYCAMERADIALOG_H
#define POVRAYCAMERADIALOG_H

#include <QDialog>

namespace Ui {
class PovRayCameraDialog;
}

class PovRayCameraDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PovRayCameraDialog(QWidget *parent = 0);
    ~PovRayCameraDialog();
    
private:
    Ui::PovRayCameraDialog *ui;
};

#endif // POVRAYCAMERADIALOG_H
