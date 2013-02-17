#ifndef SECONDDISPLAYPROJECTOR_H
#define SECONDDISPLAYPROJECTOR_H

#include "qprojector.h"
#include "../../gui/imageviewwidget.h"

/// A projector that simply uses an external
/// monitor, assumed to be a projector.
/// Despite its name, it need not be the second
/// display; it can also be used on the primary
/// or any other display hooked up to the system.
class SecondDisplayProjector : public QProjector
{
    Q_OBJECT
public:
    /// Create a new SecondDisplayProjector.
    explicit SecondDisplayProjector(QObject *parent = 0);

    /// The actual widget that does the displaying.
    class Widget: public ImageViewWidget{
    public:
        /// Set which display to project with.
        void projectOnDisplay(int n);
        /// Count the displays in the user's system.
        int getDisplayCount();
        /// Project a particular pattern.
        void projectPattern(QProjector::Pattern *p);
    };
    
signals:
    
protected:
    /// Project a pattern.
    void projectPattern(QProjector::Pattern *p);

private:
    /// The widget we're using to project.
    Widget w;
    
};

#endif // SECONDDISPLAYPROJECTOR_H
