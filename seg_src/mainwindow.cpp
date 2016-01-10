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
    displayImage = image.copy();
    // Tell the display widget to reset as well
    ui->widget->reset();
}


void MainWindow::on_selectImageButton_clicked()
{
    // Open a dialog to select a file
    QString fileName = QFileDialog::getOpenFileName(this,
                                tr("Select Image"),
                                "/home/duncan/Downloads",
                                tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));
    if (fileName == NULL) return;

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
    
    float beta = 0.001 * exp(0.0921034*ui->betaSlider->value());

    // This is where the magic happens
    QVector<QPoint> final_fore = Segmenter(&image).segment(beta, &fore, &back);

    // Reload the image to clear all the drawing from it
    displayImage = image.copy()

    QPoint pix;
    int i;
    // Note: Avoid if statements in this loop! Huge slowdown.
    for (i=0; i<final_fore.size(); i++) {
        pix = QPoint(final_fore[i].x(), final_fore[i].y());
        displayImage.setPixel(pix, qRgb(255, 255, 255));
    }


    ui->widget->setImage(&displayImage);
}

void MainWindow::on_betaSlider_sliderMoved(int position)
{
    // Scales 0 to 0.001 and 100 to ~10 
    float beta = 0.001 * exp(0.0921034*position);
    // Update the label so the user knows what value they've chosen.
    ui->betaLabel->setText(QString("Beta a(Log Scale): "+QString::number(beta)));
}
