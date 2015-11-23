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
    ui->imageLabel->clear();
    ui->imageLabel->setText("No image selected...");
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
        QImage edit_image = QImage(fileName);
        QColor pix = QColor( edit_image.pixel(0,0) );
        cout << pix;
    }
}

QVector<QPoint> MainWindow::neighbors(int x, int y)
{
    image = QImage(fileName);
    height = image.height();
    width = image.width();

    QVector<QPoint> n;

}

bool outOfBounds(int x, int y, int h, int w) {
    return ((x<w) && (y<h) && (x>=0) && (y>=0));
}
