#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "segmenter.h"

#include <iostream>
#include <cmath>

#include <QFileDialog>
#include <QPainter>

//#define FG_COLOR qRgb(12,175,243)


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
    //fileName.clear();
}


void MainWindow::on_selectImageButton_clicked()
{
    // Open a dialog to select a file
    fileName = QFileDialog::getOpenFileName(this,
                                tr("Select Image"),
                                "/home/duncan/Downloads",
                                tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));

    image.load(fileName);
    // Make a resized copy, don't resize the image itself.
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
    if (fileName.isNull()) {
        cout << "No image selected." << endl;
        return;
    }    
 
    QVector<QPoint> fore, back;
    fore = ui->widget->getForeground();
    back = ui->widget->getBackground();
    
    // This is where the magic happens
    QVector<QPoint> final_fore = Segmenter(image).segment(fore, back);

    QImage finalImage(image.width(), image.height(), QImage::Format_RGB32);
    QPoint pix;
    int r, c;

    for (c=0; c<image.width(); c++) {
        for (r=0; r<image.height(); r++) {
            pix = QPoint(c, r);
            
            if (final_fore.contains(pix)) {
                finalImage.setPixel(pix, qRgb(255, 255, 255));
            }
            else {
                finalImage.setPixel(pix, qRgb(0, 0, 0));
            }
        }
    }

    //image = finalImage.scaled(ui->widget->width(),ui->widget->height(),Qt::KeepAspectRatio);
    finalImage = finalImage.scaled(ui->widget->width(),ui->widget->height(),Qt::KeepAspectRatio);
    ui->widget->setImage(finalImage);
}
