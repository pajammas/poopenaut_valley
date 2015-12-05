#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QDebug>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>


class mywidget : public QWidget
{
    Q_OBJECT

    private:
        QImage image;
        QRgb currentSeedColor;
        QVector<QPoint> foregroundList;
        QVector<QPoint> backgroundList;

    public:
        explicit mywidget(QWidget *parent = 0);
        void setImage(QImage &image);

        // Integer representing the region. 1 for FG, 0 for BG
        void setCurrentSeed(int seed);

        void paintEvent(QPaintEvent *e);
        void mouseMoveEvent(QMouseEvent *e);
        
        QVector<QPoint> getBackground();
        QVector<QPoint> getForeground();
};

#endif // MYWIDGET_H
