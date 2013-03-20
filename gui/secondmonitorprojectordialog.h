#ifndef SECONDMONITORPROJECTORDIALOG_H
#define SECONDMONITORPROJECTORDIALOG_H

#include <QDialog>
#include "../hardware/projector/qprojector.h"

namespace Ui {
class SecondMonitorProjectorDialog;
}
/// A dialog to configure a "second-monitor" projector.
class SecondMonitorProjectorDialog : public QDialog
{
    Q_OBJECT
    
public:
    /// Create the dialog.
    explicit SecondMonitorProjectorDialog(QWidget *parent = 0);
    /// Destroy the dialog.
    ~SecondMonitorProjectorDialog();

signals:
    /// The dialog has updated the projector.
    void projectorUpdated(QProjector *);
    
private:
    /// UI
    Ui::SecondMonitorProjectorDialog *ui;
};

#endif // SECONDMONITORPROJECTORDIALOG_H
