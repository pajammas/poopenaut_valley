using namespace std;
#include <iostream>
#include <cmath>

#include <QFileDialog>
#include <QPainter>

#include <Eigen>

#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // QString initializes to NULL
    fileName = QString();
    beta = 1.0;
    currentSeedColor = qRgb(12,175,243);
    ui->widget->setCurrentSeedColor(currentSeedColor);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_resetButton_clicked()
{
    //ui->imageLabel->clear();
    //ui->imageLabel->setText("No image selected...");
    fileName.clear();
}


void MainWindow::on_selectImageButton_clicked()
{
    // Open a dialog to select a file
    fileName = QFileDialog::getOpenFileName(this,
                                tr("Select Image"),
                                "/home/duncan/Downloads",
                                tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));

    image.load(fileName);
    image = image.scaled(ui->widget->width(),ui->widget->height(),Qt::KeepAspectRatio);
    ui->widget->setImage(image);
}

void MainWindow::on_fgRadioButton_clicked()
{
    currentSeedColor = qRgb(12,175,243);
    ui->widget->setCurrentSeedColor(currentSeedColor);
}

void MainWindow::on_bgRadioButton_clicked()
{
    currentSeedColor = qRgb(236,0,16);
    ui->widget->setCurrentSeedColor(currentSeedColor);
}

void MainWindow::on_segmentButton_clicked()
{
    if (!fileName.isNull()) {
        QColor pix = QColor( image.pixel(0,0) );
        //cout << pix;
        neighbors(0, image.height());
        neighbors(image.width(), 0);
        neighbors(image.width(), image.height());
        cout << "kay\n";
        neighbors(0, image.height()-1);
        neighbors(image.width()-1, 0);
        neighbors(image.width()-1, image.height()-1);
    }
}

bool MainWindow::inBounds(int x, int y) {
    return ((x<image.width()) && (y<image.height()) && (x>=0) && (y>=0));
}

QVector<QPoint> MainWindow::neighbors(int x, int y)
{
    QVector<QPoint> n;
    // Add all neighbor pixels which are in bounds
    if (inBounds(x, y+1))  n += QPoint(x, y+1);
    if (inBounds(x, y-1))  n += QPoint(x, y-1);
    if (inBounds(x+1, y))  n += QPoint(x+1, y);
    if (inBounds(x-1, y))  n += QPoint(x-1, y);

    // For testing
    //int i;
    //for (i=0; i<n.length(); i++) cout << n[i].x() << ' ' << n[i].y() <<'\n';
    return n;
}

// Return the max of the three squared differences.
int norm(QColor i, QColor j)
{
    int r = pow(i.red() - j.red(), 2);
    int g = pow(i.green() - j.green(), 2);
    int b = pow(i.blue() - j.blue(), 2);
    return max(r, max(g, b));
}

// This function will set sigma, as well as negBetaSigma for later use.
void MainWindow::setSigma() {
    // Sigma is the maximum norm of the image.
    sigma = 0;
    // Iterators
    int r, c, n;
    QColor pix, neigh;
    // For each pixel in the image:
    for (r=0; r<image.height(); r++) {
        for (c=0; c<image.width(); c++) {
            // Get the pixel color and its neighbors' coordinates
            pix = image.pixel(r,c);
            QVector<QPoint> neighs = neighbors(r, c);
            // For each neighbor:
            for (n=0; n<neighs.length(); n++) {
                // Get the neighbor's color
                neigh = image.pixel(neighs[n]);
                // If the norm is greater, set it as the new max.
                sigma = max(sigma, norm(pix, neigh));
            }
        }
    }
    // Make sure we never divide by zero.
    if (sigma != 0)
        negBetaSigma = (-1 * beta) / sigma;
    else
        // If sigma = 0, the image is all the same color.
        // Weights don't matter as long as they're all equal.
        negBetaSigma = 0;
}

// Simple equation from the paper.
float MainWindow::weight(QColor i, QColor j)
{
    return exp(negBetaSigma * norm(i,j)) + 0.000001;
}
