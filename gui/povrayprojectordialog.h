#ifndef POVRAYPROJECTORDIALOG_H
#define POVRAYPROJECTORDIALOG_H

#include <QDialog>

#include "../hardware/projector/povrayprojector.h"

namespace Ui {
class PovRayProjectorDialog;
}

/// A dialog for setting up the POV-Ray projector.
class PovRayProjectorDialog : public QDialog
{
    Q_OBJECT
    
public:
    /// Create the dialog.
    explicit PovRayProjectorDialog(QWidget *parent = 0);
    /// Destroy the dialog.
    ~PovRayProjectorDialog();

signals:
    /// The user has updated the projector's settings.
    void projectorUpdated(QProjector *);

public slots:
    /// The user has accepted the dialog.
    void accept();
    
private:
    /// UI
    Ui::PovRayProjectorDialog *ui;
    /// Projector
    PovRayProjector *projector;
};

#endif // POVRAYPROJECTORDIALOG_H
