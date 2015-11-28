#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <cmath>

#include <QFileDialog>
#include <QPainter>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // QString initializes to NULL
    fileName = QString();
    // beta = 1.0;   //talked with Josito and he suggested to use 400
    beta = 400.0;

    currentSeedColor = qRgb(12, 175, 243);
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
    //QImage display_img = image.scaled(ui->widget->width(),ui->widget->height(),Qt::KeepAspectRatio);
    //ui->widget->setImage(display_img);
    ui->widget->setImage(image);
}

void MainWindow::on_fgRadioButton_clicked()
{
    currentSeedColor = qRgb(12, 175, 243);
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
        //qDebug() << image.width() << image.height() <<endl;
        QVector<QPoint> test_fore = ui->widget->getForeground();
        QVector<QPoint> test_back = ui->widget->getBackground();

        setSigma();
        //cout << negBetaSigma;

        MatrixXf L = getLMatrix();
        MatrixXf I = getIMatrix(test_fore, test_back);
        VectorXf B = getBVector(test_fore, test_back);
        MatrixXf A = I + L*L;

        // Apparently there are other options for an Ax=b solver.
        // We could take some time to choose the best.
        VectorXf X = A.colPivHouseholderQr().solve(B);
        //cout << X << endl;

        QVector<QPoint> final_fore, final_back;
        QPoint pix;
        QImage finalImage(image.width(), image.height(), QImage::Format_RGB32);
        int i;
        for (i=0; i<X.size(); i++) {
            // Integer division will give the value rounded down
            pix = QPoint(i%image.width(), i/image.width());
            cout << pix.x() << ' ' << pix.y() << ' '<< X[i] << endl;
            if (X[i] < 0){
                final_back += pix;
                finalImage.setPixel(pix, qRgb(255, 255, 255));
            }
            else {
                final_fore += pix;
                finalImage.setPixel(pix, qRgb(0, 0, 0));
            }
        }

        image = finalImage.scaled(ui->widget->width(),ui->widget->height(),Qt::KeepAspectRatio);
        ui->widget->setImage(image);
        //cout << final_back << endl;
        //cout << final_fore << endl;
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
    /*
    neighbors(0, image.height());
    neighbors(image.width(), 0);
    neighbors(image.width(), image.height());
    cout << "kay\n";
    neighbors(0, image.height()-1);
    neighbors(image.width()-1, 0);
    neighbors(image.width()-1, image.height()-1);
    */
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
    sigma = 0.1;
    /* According to paper sigma should be equal to 0.1 ups ^__^
     * I mean good try and well done, and girls will like your for loops
     *  with c and r instead of i,j buuuuuut.... you knoooow....
     *
    // Iterators
    int r, c, n;
    QColor pix, neigh;
    // For each pixel in the image:
    for (c=0; c<image.width(); c++) {
        for (r=0; r<image.height(); r++) {

            // Get the pixel color and its neighbors' coordinates
            pix = image.pixel(c,r);
            QVector<QPoint> neighs = neighbors(c, r);
            // For each neighbor:
            for (n=0; n<neighs.length(); n++) {
                // Get the neighbor's color
                neigh = image.pixel(neighs[n]);
                // If the norm is greater, set it as the new max.
                sigma = max(sigma, norm(pix, neigh));
            }
        }
    }
    */
    // Make sure we never divide by zero.
    if (sigma != 0)
        negBetaSigma = (-1 * beta) / sigma;
    else
        // If sigma = 0, the image is all the same color.
        // Weights don't matter as long as they're all equal.
        negBetaSigma = 0;
}

// Simple equation from the paper.1li
float MainWindow::weight(QColor i, QColor j)
{
    return exp(negBetaSigma * norm(i,j)) + 0.000001;
}

// L = D - W
MatrixXf MainWindow::getLMatrix()
{
    int h = image.height();
    int w = image.width();
    // Dynamic initialization of square matrix, length = rows*cols
    MatrixXf L = MatrixXf::Zero(h*w, h*w);

    // Iterators
    int r, c, n;
    QColor pix, neigh;
    // For each pixel in the image:
    for (c=0; c<w; c++) {
        for (r=0; r<h; r++) {

            // Get the pixel color and its neighbors' coordinates
            pix = image.pixel(c,r);
            QVector<QPoint> neighs = neighbors(c, r);

            //qDebug() << c << " " << r << endl;
            // For each neighbor:
            for (n=0; n<neighs.length(); n++) {
                // Get the neighbor's color
                neigh = image.pixel(neighs[n]);

                double currentWeight = weight(pix, neigh);
                // This is an element of the W matrix. Negative.
                L(r*w+c, neighs[n].y()*w + neighs[n].x()) = -currentWeight;
                // Element of D matrix. Accumulate all the weights of pix.
                L(r*w+c, r*w+c) += currentWeight;
                // To avoid more calls to weight(), we could use the W entry.
            }
        }
    }
    return L;
}


MatrixXf MainWindow::getIMatrix(QVector<QPoint> fore, QVector<QPoint> back) {
    int w = image.width();
    int N = image.height() * w;
    MatrixXf I = MatrixXf::Zero(N, N);

    int i, r, c;
    for(i=0; i<fore.length(); i++) {
        r = fore[i].y();
        c = fore[i].x();
        I(r*w+c, r*w+c) = 1.0;
    }
    for(i=0; i<back.length(); i++) {
        r = back[i].y();
        c = back[i].x();
        I(r*w+c, r*w+c) = 1.0;
    }

    return I;
}

// We could get this at the same time as I, for elegancy/speed.
VectorXf MainWindow::getBVector(QVector<QPoint> fore, QVector<QPoint> back) {
    int w = image.width();
    VectorXf B = VectorXf::Zero(image.height() * w);

    int i, r, c;
    for(i=0; i<fore.length(); i++) {
        r = fore[i].y();
        c = fore[i].x();
        B(r*w+c) = 1.0;
    }
    for(i=0; i<back.length(); i++) {
        r = back[i].y();
        c = back[i].x();
        B(r*w+c) = -1.0;
    }

    return B;
}
