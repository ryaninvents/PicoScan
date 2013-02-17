#ifndef FILEPROJECTORDIALOG_H
#define FILEPROJECTORDIALOG_H

#include <QDialog>

namespace Ui {
class FileProjectorDialog;
}

class FileProjectorDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit FileProjectorDialog(QWidget *parent = 0);
    ~FileProjectorDialog();
    
private:
    Ui::FileProjectorDialog *ui;
};

#endif // FILEPROJECTORDIALOG_H
