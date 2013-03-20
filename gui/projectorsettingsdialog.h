#ifndef PROJECTORSETTINGSDIALOG_H
#define PROJECTORSETTINGSDIALOG_H

#include <QDialog>
#include "../hardware/projector/qprojector.h"

#define PROJECTOR_TYPE_MONITOR 0
#define PROJECTOR_TYPE_POVRAY 1
#define PROJECTOR_TYPE_FILE 2

namespace Ui {
class ProjectorSettingsDialog;
}
/// A dialog for setting up the projector.
class ProjectorSettingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    /// Create the dialog
    explicit ProjectorSettingsDialog(QWidget *parent = 0);
    /// Destroy the dialog
    ~ProjectorSettingsDialog();

signals:
    /// User has updated the projector preferences.
    void projectorUpdated(QProjector*);
    
public slots:

    /// Projector type has changed
    void projectorTypeChanged(int t);

    /// Projector has been updated
    void updateProjector(QProjector *p);

private:

    /// UI
    Ui::ProjectorSettingsDialog *ui;

    /// Current projector
    QProjector *projector;

    /// Detailed settings dialog. This deals
    /// with the settings that are specific
    /// to each subclass.
    QDialog *settingsDialog;
};

#endif // PROJECTORSETTINGSDIALOG_H
