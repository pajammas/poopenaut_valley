#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QPainter>
//#include <string>
#include <iostream>
using namespace std;

#include <Eigen>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // QString initializes to NULL
    fileName = QString();
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

bool inBounds(int h, int w, int x, int y) {
    return ((x<w) && (y<h) && (x>=0) && (y>=0));
}

QVector<QPoint> MainWindow::neighbors(int x, int y)
{
    int h = image.height();
    int w = image.width();

    QVector<QPoint> n;
    if (inBounds(h, w, x, y+1))  n += QPoint(x, y+1);
    if (inBounds(h, w, x, y-1))  n += QPoint(x, y-1);
    if (inBounds(h, w, x+1, y))  n += QPoint(x+1, y);
    if (inBounds(h, w, x-1, y))  n += QPoint(x-1, y);

    int i;
    for (i=0; i<n.length(); i++) cout << n[i].x() << ' ' << n[i].y() <<'\n';
    return n;
}
