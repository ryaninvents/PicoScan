#include "projectionscreen.h"

#include <QApplication>
#include <QDesktopWidget>

ProjectionScreen::ProjectionScreen(QWidget *parent) :
    ImageViewWidget(parent)
{
    setWindowTitle("Projection screen");
    setCursor(QCursor(Qt::BlankCursor));

    QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::black);
    setAutoFillBackground(true);
    setPalette(pal);

    setContentsMargins(0,0,0,0);
}

/*
        The purpose of this function is to convert an unsigned
        binary number to reflected binary Gray code.

        The operator >> is shift right. The operator ^ is exclusive or.
*/
unsigned int ProjectionScreen::binaryToGray(unsigned int num)
{
        return (num >> 1) ^ num;
}

/*
        The purpose of this function is to convert a reflected binary
        Gray code number to a binary number.
*/
unsigned int ProjectionScreen::grayToBinary(unsigned int num)
{
    unsigned int numBits = 8 * sizeof(num);
    unsigned int shift;
    for (shift = 1; shift < numBits; shift = 2 * shift)
    {
        num = num ^ (num >> shift);
    }
    return num;
}


QImage createGray(unsigned int width,
                  unsigned int height,
                  unsigned int n,
                  bool invert){
    QImage out(width,height,QImage::Format_ARGB32);
    unsigned int values[width];
    unsigned int x,y,gray;
    unsigned int mask = 1<<n;

    for (x = 0; x < width; x++) {
        gray = ProjectionScreen::binaryToGray(x);
        if(!invert)
            values[x] = (gray & mask) ? 0:255;
        else
            values[x] = (gray & mask) ? 255:0;
    }

    for (y = 0; y < height; y++) {
            QRgb *destrow = (QRgb*)out.scanLine(y);
            for (x = 0; x < width; ++x) {
                destrow[x] = qRgba(0,0,0, values[x]);
            }
    }

    return out;
}


void ProjectionScreen::projectOnDisplay(int n)
{
    if(n>=getDisplayCount()) return;
    QRect screenres = QApplication::desktop()->screenGeometry(n);
    move(QPoint(screenres.x(),screenres.y()));
    resize(screenres.width(),screenres.height());
    showFullScreen();
}

int ProjectionScreen::getDisplayCount()
{
    return QApplication::desktop()->screenCount();
}

void ProjectionScreen::projectBinary(int bit, bool inverted)
{
    QImage im = createGray(640,480,bit,inverted);
    displayImage(im);
}

