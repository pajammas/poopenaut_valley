#include "mywidget.h"
#include <QList>

mywidget::mywidget(QWidget *parent) : QWidget(parent)
{

}

void mywidget::paintEvent(QPaintEvent * e) {
    QPainter painter(this);

    painter.drawImage(0, 0, image);

    QWidget::paintEvent(e);
}

void mywidget::setImage(QImage& imageIn) {
    image = imageIn; //in order for mainwindow to ACTUALLY paint a pic
    update();
}

void mywidget::setCurrentSeedColor(QRgb currentSeedIn) {
    currentSeed = currentSeedIn;
}

void mywidget::mouseMoveEvent(QMouseEvent * e) {

   // return;

    int x = e->x();
    int y = e->y();
    qDebug()<< e->type()<<endl;
    qDebug() << x << " " << y << endl;

    if ((e->buttons()==Qt::LeftButton)&&(currentSeed == qRgb(12,175,243)))
        {
            image.setPixel(QPoint(x, y), currentSeed);
            image.setPixel(QPoint(x-1, y), currentSeed);
            image.setPixel(QPoint(x+1, y), currentSeed);
            image.setPixel(QPoint(x, y-1), currentSeed);
            image.setPixel(QPoint(x+1, y+1), currentSeed);


            ForegroundX.append(x); //list with X seeds of foreground
            ForegroundY.append(y);
            update();
        }

    // sooo here we have a problem:if you just press the right mouse button you call the following
    // if-statement, but the color of the pen is blue instead of red. in order to change the color we
    //  have to work with mouse right click event which I don't know how to manage.
    if ((e->buttons()==Qt::RightButton)||(currentSeed == qRgb(236,0,16)))
    {
        image.setPixel(QPoint(x, y), currentSeed);
        image.setPixel(QPoint(x-2, y), currentSeed);
        image.setPixel(QPoint(x+2, y), currentSeed);
        image.setPixel(QPoint(x, y-2), currentSeed);
        image.setPixel(QPoint(x, y+2), currentSeed);
        BackgroundX.append(x);
        BackgroundY.append(y);
        update();
    }

}



