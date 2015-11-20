using namespace std;
#include <string>
#include <iostream>

#include <QFileDialog>
#include <Eigen>

#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_resetButton_clicked()
{
    ui->imageLabel->clear();
    ui->imageLabel->setText("No image selected...");
}


void MainWindow::on_selectImageButton_clicked()
{
    // Open a dialog to select a file
    QString fileName = QFileDialog::getOpenFileName(this,
                        tr("Select Image"), "/home/duncan/Downloads", tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));
    // Display the image in the window
    QPixmap image(fileName);
    QPixmap ScaledIm = image.scaled(ui->imageLabel->width(),ui->imageLabel->height(),Qt::KeepAspectRatio);
    ui->imageLabel->setPixmap(ScaledIm);

    // Resize the window to fit the image
    //ui->centralWidget->adjustSize();
    //this->adjustSize();

}

void MainWindow::on_fgRadioButton_clicked()
{

}
