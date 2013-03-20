#ifndef POSITIONWIDGET_H
#define POSITIONWIDGET_H

#include <QWidget>

namespace Ui {
class PositionWidget;
}

class PositionWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PositionWidget(QWidget *parent = 0);
    ~PositionWidget();
    
private:
    Ui::PositionWidget *ui;
};

#endif // POSITIONWIDGET_H
