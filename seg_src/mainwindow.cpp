#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "segmenter.h"
#include "mywidget.h"

#include <cmath>
#include <iostream>

#include <QFileDialog>


QPalette seedPalettes[NUM_SEEDS] = {QPalette(Qt::red),
                                    QPalette(Qt::green),
                                    QPalette(Qt::blue),
                                    QPalette(Qt::cyan),
                                    QPalette(Qt::magenta),
                                    QPalette(Qt::yellow)};


MainWindow::MainWindow(QWidget *parent) :    
    QMainWindow(parent),
    ui(new Ui::MainWindow)
    {
        ui->setupUi(this);

        image = QImage();
        displayImage = QImage();
        
        mapper = new QSignalMapper();
        makeSeedButtons();
    }

MainWindow::~MainWindow()
{
    for(int i=0; i<NUM_SEEDS; i++)
    {
        delete seedButtons[i];
    }
    delete mapper;
    delete ui;
}

void MainWindow::makeSeedButtons() {   
    for(int i=0; i<NUM_SEEDS; i++)
    {
        seedButtons[i] = new QRadioButton(QString("Seed %1").arg(i+1));
        // Color the button
        seedButtons[i]->setPalette(seedPalettes[i]);

        // Map this button to its index
        mapper->connect(seedButtons[i], SIGNAL(clicked()), SLOT(map()));
        mapper->setMapping(seedButtons[i], i);
        
        ui->seedLayout->addWidget(seedButtons[i]);
    }
    // Connect the mapped index to the setSeed(int) function.
    ui->tabWidget->controlsTab->connect(mapper, SIGNAL(mapped(int)), SLOT(setCurrentSeed(int)));
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

    QPoint pix;
    int i;
    // Note: Avoid if statements in this loop! Huge slowdown.
    for (i=0; i<final_fore.size(); i++) {
        pix = QPoint(final_fore[i].x(), final_fore[i].y());
        displayImage.setPixel(pix, qRgb(255, 255, 255));
    }


    ui->widget->setImage(&displayImage);
}

void MainWindow::on_resetButton_clicked()
{
    displayImage = image.copy();
    // Tell the display widget to reset as well
    ui->widget->reset();
}


void MainWindow::on_betaSlider_valueChanged(int value)
{
    // Scales (0, 100) to (0.001, 10)
    float beta = 0.001 * exp(0.0921034*value);
    // Update the label so the user knows what value they've chosen.
    ui->betaLabel->setText(QString("Beta (Log Scale): "+QString::number(beta)));
}
