#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class AboutDialog;
}
/// A simple "about" dialog where I get to brag
/// about our team.
class AboutDialog : public QDialog
{
    Q_OBJECT
public:
    /// Initialize the dialog
    explicit AboutDialog(QWidget *parent = 0);
    /// Destroy the dialog
    ~AboutDialog();
    
private:
    /// UI
    Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_H
