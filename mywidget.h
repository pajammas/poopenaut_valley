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
    QList<int> ForegroundX;
    QList<int> ForegroundY;
    QList<int> BackgroundX;
    QList<int> BackgroundY;
    void ReturnBackgroundX(int[]);
    void ReturnBackgroundY(int[]);
    void ReturnForegroundX(int[]);
    void ReturnForegroundY(int[]);


public:
    explicit mywidget(QWidget *parent = 0);

    void setImage(QImage& image);

    void setCurrentSeedColor(QRgb currentSeed);

    void paintEvent(QPaintEvent * e);

    void mouseMoveEvent(QMouseEvent * e);

signals:

public slots:
};

#endif // MYWIDGET_H
