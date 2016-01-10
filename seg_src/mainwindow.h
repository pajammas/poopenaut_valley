#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>

// Main window has access to the UI
namespace Ui {
    class MainWindow;
}

// Middleman to connect all the other parts of the project
class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        // Optional argument makes this serve as a default constructor.
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    // Qt slots for each button to provide the desired action
    private slots:
        void on_resetButton_clicked();

        void on_selectImageButton_clicked();

        void on_segmentButton_clicked();

        void on_fgRadioButton_clicked();
        void on_bgRadioButton_clicked();

        // This triggers each time the slider moves
        void on_betaSlider_sliderMoved(int);



private:
        Ui::MainWindow *ui;
        // Image is the master copy; 
        // display image is changed and passed to drawing widget
        QImage image;
        QImage displayImage;

};

#endif // MAINWINDOW_H
