#ifndef GRAPHVIEWWIDGET_H
#define GRAPHVIEWWIDGET_H

#include <QWidget>

namespace Ui {
class GraphViewWidget;
}

class GraphViewWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit GraphViewWidget(QWidget *parent = 0);
    ~GraphViewWidget();
    
private:
    Ui::GraphViewWidget *ui;
};

#endif // GRAPHVIEWWIDGET_H
