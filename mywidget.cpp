#include "mywidget.h"
#include <QList>

#define FG_COLOR qRgb(12, 175, 243)
#define BG_COLOR qRgb(236, 0, 16)


mywidget::mywidget(QWidget *parent) : QWidget(parent)
{}


void mywidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);

    painter.drawImage(0, 0, image);

    QWidget::paintEvent(e);
}


void mywidget::setImage(QImage &imageIn)
{
    image = imageIn; //in order for mainwindow to ACTUALLY paint a picture
    update();
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

    if (!foregroundList.contains(p) &&
        !backgroundList.contains(p) &&
        (e->buttons() == Qt::LeftButton))
    {
            image.setPixel(p, currentSeedColor);
            image.setPixel(QPoint(x-1, y), currentSeedColor);
            image.setPixel(QPoint(x+1, y), currentSeedColor);
            image.setPixel(QPoint(x,   y-1), currentSeedColor);
            image.setPixel(QPoint(x+1, y+1), currentSeedColor);
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
