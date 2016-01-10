#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QDebug>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>


class mywidget : public QWidget
{
    Q_OBJECT

    public:
        // Optional argument makes this serve as a default constructor.
        explicit mywidget(QWidget *parent = 0);
        // Image cannot be const, because we want to draw on it.
        void setImage(QImage *);

        QVector<QPoint> getBackground();
        QVector<QPoint> getForeground();
        void reset();

    public slots:
        // Integer representing the region. 1 for FG, 0 for BG
        void setCurrentSeed(int);
        

    private:
        QImage *image;
        QRgb currentSeedColor;
        QVector<QPoint> foregroundList;
        QVector<QPoint> backgroundList;

        bool inBounds(int, int);
        void paintEvent(QPaintEvent *);
        void mouseMoveEvent(QMouseEvent *);
};

#endif // MYWIDGET_H
