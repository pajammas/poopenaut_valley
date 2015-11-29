#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "segmenter.h"

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
    currentSeedColor = qRgb(12,175,243);
    ui->widget->setCurrentSeedColor(currentSeedColor);
}

MainWindow::~MainWindow()
    { delete ui; }

// Make this work again?
// Maybe have a "clear seeds" button as well?
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
    // Make a resized copy, don't resize the image itself.
    QImage display_img = image.scaled(ui->widget->width(),ui->widget->height(),Qt::KeepAspectRatio);
    ui->widget->setImage(display_img);
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

    //qDebug() << image.width() << image.height() <<endl;
    QVector<QPoint> test_fore, test_back;
    test_fore += QPoint(0,0);
    test_back += QPoint(3,2);
    test_back += QPoint(2,3);

    QVector<QPoint> final_fore;
    final_fore = Segmenter(fileName).segment(test_fore, test_back);
 
    // Now do something with it!!
}
