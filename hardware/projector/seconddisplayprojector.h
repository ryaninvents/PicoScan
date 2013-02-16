#ifndef SECONDDISPLAYPROJECTOR_H
#define SECONDDISPLAYPROJECTOR_H

#include "qprojector.h"
#include "../../gui/imageviewwidget.h"

class SecondDisplayProjector : public QProjector
{
    Q_OBJECT
public:
    explicit SecondDisplayProjector(QObject *parent = 0);

    class Widget: public ImageViewWidget{
    public:
        void projectOnDisplay(int n);
        int getDisplayCount();
        void projectPattern(QProjector::Pattern *p);
    };
    
signals:
    
public slots:
    void projectPattern(QProjector::Pattern *p,
                        ImageDescriptor desc);

private:
    Widget w;
    
};

#endif // SECONDDISPLAYPROJECTOR_H
