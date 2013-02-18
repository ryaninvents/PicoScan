#ifndef FILEPROJECTORDIALOG_H
#define FILEPROJECTORDIALOG_H

#include <QDialog>

namespace Ui {
class FileProjectorDialog;
}

/// A dialog for configuring a projector, used at some
/// point in the past to capture a series of images. The
/// concept is that the user will also use a FileCamera
/// to process a full set of images.
/// \todo implement FileCamera
class FileProjectorDialog : public QDialog
{
    Q_OBJECT
    
public:
    /// Create the dialog
    explicit FileProjectorDialog(QWidget *parent = 0);
    /// Destroy the dialog
    ~FileProjectorDialog();
    
private:
    /// UI
    Ui::FileProjectorDialog *ui;
};

#endif // FILEPROJECTORDIALOG_H
