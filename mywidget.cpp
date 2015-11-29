#include "mywidget.h"
#include <QList>

mywidget::mywidget(QWidget *parent) : QWidget(parent)
{

}

void mywidget::paintEvent(QPaintEvent * e)
{
    QPainter painter(this);

    painter.drawImage(0, 0, image);

    QWidget::paintEvent(e);
}

void mywidget::setImage(QImage& imageIn)
{
    image = imageIn; //in order for mainwindow to ACTUALLY paint a picture
    update();
}

void mywidget::setCurrentSeedColor(QRgb currentSeedIn)
{
    currentSeed = currentSeedIn;
}

void mywidget::mouseMoveEvent(QMouseEvent * e)
{
    int x = e->x();
    int y = e->y();

    QImage display_image = image.copy();

    if (e->buttons() == Qt::LeftButton)
    {
        image.setPixel(QPoint(x, y), currentSeed);
        image.setPixel(QPoint(x - 1, y), currentSeed);
        image.setPixel(QPoint(x + 1, y), currentSeed);
        image.setPixel(QPoint(x, y - 1), currentSeed);
        image.setPixel(QPoint(x + 1, y + 1), currentSeed);
        update();

        if (currentSeed == qRgb(12, 175, 243))
        {
            foregroundList.append(QPoint(x, y));
        }
        else
        {
            backgroundList.append(QPoint(x, y));
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
