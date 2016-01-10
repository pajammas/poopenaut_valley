#include "mywidget.h"
#include <QList>


// Constructor and default constructor
mywidget::mywidget(QWidget *parent) :
    QWidget(parent)
    {
        image = NULL;
        // Foreground seed is selected on startup
        FG_COLOR = qRgb(12, 175, 243);
        BG_COLOR = qRgb(236, 0, 16);
        setCurrentSeed(1);

        // initialize as empty lists
        foregroundList = QVector<QPoint>();
        backgroundList = QVector<QPoint>();
    }
    
// Image reference cannot be const, because we want to draw on it.
void mywidget::setImage(QImage *imageIn)
{
    image = imageIn;
    update();
}

// When the image is drawn upon, update it to show changes
void mywidget::paintEvent(QPaintEvent *e)
{
    if (image != NULL)
    {
        QPainter painter(this);
        painter.drawImage(0, 0, *image);
    }
    
    QWidget::paintEvent(e);
}

// Helper function to make sure everything is in bounds
bool mywidget::inBounds(int x, int y)
{
    return((x >= 0) &&
           (y >= 0) &&
           (x < (image->width())) &&
           (y < (image->height())) );
}


// Simple setter function
void mywidget::setCurrentSeed(int color)
{
    if (color == 1) currentSeedColor = FG_COLOR;
    else            currentSeedColor = BG_COLOR;
}

// If the user draws on the picture, add the pixel to a list
void mywidget::mouseMoveEvent(QMouseEvent *e)
{
    int x = e->x();
    int y = e->y();
    QPoint p = QPoint(x, y);

    // If there's an image, this pixel is inbounds,
    // and not already in a list, add it to a list
    if (image != NULL &&
        inBounds(x, y) &&
        !foregroundList.contains(p) &&
        !backgroundList.contains(p) &&
        e->buttons() == Qt::LeftButton )
    {
            // Set this pixel to be the seed color
            image->setPixel(p, currentSeedColor);
            
            // Set 4-neighbor pixels as well for visibility
            int i;
            for (i=-1; i<2; i+=2) {
                if (inBounds(x+i, y))
                    image->setPixel(QPoint(x+i, y), currentSeedColor);
                if (inBounds(x, y+i))
                    image->setPixel(QPoint(x, y+i), currentSeedColor);
            }
            update();

            // Add to FG or BG list
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

// Simple getter functions
QVector<QPoint> mywidget::getBackground()
{
    return backgroundList;
}

QVector<QPoint> mywidget::getForeground()
{
    return foregroundList;
}

// Clear previous drawing from the image and empty the lists
void mywidget::reset() {
    update();
    foregroundList = QVector<QPoint>();
    backgroundList = QVector<QPoint>();
}
