#ifndef ROTATIONWIDGET_H
#define ROTATIONWIDGET_H

#include <QWidget>

namespace Ui {
class RotationWidget;
}

class RotationWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit RotationWidget(QWidget *parent = 0);
    ~RotationWidget();
    
private:
    Ui::RotationWidget *ui;
};

#endif // ROTATIONWIDGET_H
