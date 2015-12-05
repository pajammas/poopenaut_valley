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
        bool inBounds(int, int);
        void paintEvent(QPaintEvent *);
        void mouseMoveEvent(QMouseEvent *);

    public:
        explicit mywidget(QWidget *parent = 0);
        void setImage(QImage &);

        // Integer representing the region. 1 for FG, 0 for BG
        void setCurrentSeed(int);
        
        QVector<QPoint> getBackground();
        QVector<QPoint> getForeground();
};

#endif // MYWIDGET_H
