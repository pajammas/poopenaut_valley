#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "segmenter.h"
#include "mywidget.h"

#include <cmath>
#include <iostream>

#include <QFileDialog>



MainWindow::MainWindow(QWidget *parent) :    
    QMainWindow(parent),
    ui(new Ui::MainWindow)
    {
        ui->setupUi(this);
        image = QImage();
        displayImage = QImage();
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
    QString fileName = QFileDialog::getOpenFileName(this,
                                tr("Select Image"),
                                "/home/duncan/Downloads",
                                tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));

    image.load(fileName);
    // Make a resized copy, don't resize the image itself?
    image = image.scaled(ui->widget->width(),ui->widget->height(),Qt::KeepAspectRatio);
    displayImage = image.copy();
    ui->widget->setImage(&displayImage);
}

void MainWindow::on_fgRadioButton_clicked()
{
    ui->widget->setCurrentSeed(1);
}

void MainWindow::on_bgRadioButton_clicked()
{
    ui->widget->setCurrentSeed(0);
}

void MainWindow::on_segmentButton_clicked()
{
    if (image.isNull()) {
        cout << "No image selected." << endl;
        return;
    }    
 
    QVector<QPoint> fore, back;
    fore = ui->widget->getForeground();
    back = ui->widget->getBackground();
    
    // This is where the magic happens
    QVector<QPoint> final_fore = Segmenter(&image).segment(&fore, &back);
    QPoint pix;
    int i;
    for (i=0; i<final_fore.size(); i++) {
        pix = QPoint(final_fore[i].x(), final_fore[i].y());
        if (final_fore.contains(pix)) {
            displayImage.setPixel(pix, qRgb(255, 255, 255));
        }
    }

    ui->widget->setImage(&displayImage);
}
