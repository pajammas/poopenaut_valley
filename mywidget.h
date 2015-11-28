#ifndef MYWIDGET_H
#define MYWIDGET_H
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QWidget>

class mywidget : public QWidget
{
    Q_OBJECT

    private:
        QImage image;
        QRgb currentSeed;
        QVector<QPoint> foregroundList;
        QVector<QPoint> backgroundList;

    public:
        explicit mywidget(QWidget *parent = 0);

        void setImage(QImage& image);

        void setCurrentSeedColor(QRgb currentSeed);

        QVector<QPoint> getBackground();

        QVector<QPoint> getForeground();

        void paintEvent(QPaintEvent * e);

        void mouseMoveEvent(QMouseEvent * e);
};

#endif // MYWIDGET_H
