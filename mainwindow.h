#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Should we put as many includes as possible here?

#include <QMainWindow>
#include <QList>

using namespace std;


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
        void on_resetButton_clicked();

        void on_selectImageButton_clicked();

        void on_segmentButton_clicked();

        void on_fgRadioButton_clicked();
        void on_bgRadioButton_clicked();


    private:
        Ui::MainWindow *ui;
        QImage image;
        QImage displayImage;

};

#endif // MAINWINDOW_H
