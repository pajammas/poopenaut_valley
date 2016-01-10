#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Should we put as many includes as possible here?

#include <QMainWindow>
#include <QList>
#include <QRadioButton>
#include <QSignalMapper>


using namespace std;



// Remove this if possible
static const int NUM_SEEDS = 6;


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        // Optional argument makes this serve as a default constructor.
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private slots:
        void on_selectImageButton_clicked();

        void on_segmentButton_clicked();

        void on_resetButton_clicked();

        void on_betaSlider_valueChanged(int value);



    private:
        Ui::MainWindow *ui;
        QImage image;
        QImage displayImage;

        QRadioButton *seedButtons[NUM_SEEDS];
        QSignalMapper *mapper;
        // Helper function to set up the seed buttons
        void makeSeedButtons();


};

#endif // MAINWINDOW_H
