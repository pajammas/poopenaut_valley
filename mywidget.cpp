#define FG_COLOR qRgb(12, 175, 243)
#define BG_COLOR qRgb(236, 0, 16)

#include "mywidget.h"
#include <QList>

// Take this out when done debugging
using namespace std;
#include <iostream>
#include <QDebug>
#include <QString>

mywidget::mywidget(QWidget *parent) :
    QWidget(parent)
    {
        image = NULL;
        // Foreground seed is selected on startup
        setCurrentSeed(1);
        foregroundList = QVector<QPoint>();
        backgroundList = QVector<QPoint>();
    }
    
// Image cannot be const, because we want to draw on it.
void mywidget::setImage(QImage *imageIn)
{
    image = imageIn;
    update();
}

void mywidget::paintEvent(QPaintEvent *e)
{
    if (image != NULL)
    {
        QPainter painter(this);
        painter.drawImage(0, 0, *image);
    }
    
    QWidget::paintEvent(e);
}

bool mywidget::inBounds(int x, int y)
{
    return((x >= 0) 
        && (y >= 0) 
        && (x < (image->width()))
        && (y < (image->height())) );
}

void mywidget::setCurrentSeed(int color)
{
    if (color == 1) currentSeedColor = FG_COLOR;
    else            currentSeedColor = BG_COLOR;
}


void mywidget::mouseMoveEvent(QMouseEvent *e)
{
    int x = e->x();
    int y = e->y();

    QPoint p = QPoint(x, y);

    if (image != NULL &&
        inBounds(x, y) &&
        !foregroundList.contains(p) &&
        !backgroundList.contains(p) &&
        (e->buttons() == Qt::LeftButton))
    {
            image->setPixel(p, currentSeedColor);
            image->setPixel(QPoint(x-1, y), currentSeedColor);
            image->setPixel(QPoint(x+1, y), currentSeedColor);
            image->setPixel(QPoint(x,   y-1), currentSeedColor);
            image->setPixel(QPoint(x+1, y+1), currentSeedColor);
            update();

            if (currentSeedColor == FG_COLOR)
            {
                foregroundList.append(p);
            }
            else
            {
                backgroundList.append(p);
            }
    }
}

QVector<QPoint> mywidget::getBackground()
{
    return backgroundList;
}

QVector<QPoint> mywidget::getForeground()
{
    return foregroundList;
}
