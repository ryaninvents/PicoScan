#ifndef GRAPHVIEWWIDGET_H
#define GRAPHVIEWWIDGET_H

#include <QWidget>
#include <opencv2/core/core.hpp>

namespace Ui {
class GraphViewWidget;
}

class GraphViewWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit GraphViewWidget(QWidget *parent = 0);
    ~GraphViewWidget();

public slots:
    void plotLine(QString headers, cv::Mat data);
    void setXRange(int xMin, int xMax);
    
private:
    Ui::GraphViewWidget *ui;
};

#endif // GRAPHVIEWWIDGET_H
