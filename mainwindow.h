#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_resetButton_clicked();

    void on_selectImageButton_clicked();

    void on_fgRadioButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
