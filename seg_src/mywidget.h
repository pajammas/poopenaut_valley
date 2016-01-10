#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

// DRAWING WIDGET
class mywidget : public QWidget
{
    Q_OBJECT

    public:
        // Optional argument makes this serve as a default constructor.
        explicit mywidget(QWidget *parent = 0);
        // Image cannot be const, because we want to draw on it.
        void setImage(QImage *);

        // Integer representing the region. 1 for FG, 0 for BG
        void setCurrentSeed(int);
        // Getter functions
        QVector<QPoint> getBackground();
        QVector<QPoint> getForeground();
        // Empty the FG/BG lists
        void reset();

    private:
        // Reference to the image loaded by MainWindow
        QImage *image;
        QRgb currentSeedColor;
        QRgb FG_COLOR;
        QRgb BG_COLOR;
        QVector<QPoint> foregroundList;
        QVector<QPoint> backgroundList;

        // Helper function for checks during painting
        bool inBounds(int, int);

        // Make updates when the user takes action
        void paintEvent(QPaintEvent *);
        void mouseMoveEvent(QMouseEvent *);
};

#endif // MYWIDGET_H
