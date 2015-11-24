#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>

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

        void on_bgRadioButton_clicked();

        void on_segmentButton_clicked();

    private:
        Ui::MainWindow *ui;
        QString fileName;
        QImage image;
        QRgb currentSeedColor;
        float beta;
        // Don't really need sigma saved.
        int sigma;
        // Only compute -1*beta/sigma once.
        float negBetaSigma;

        QVector<QPoint> neighbors(int, int);
        bool inBounds(int, int);
        void setSigma();
        float weight(QColor, QColor);



};

#endif // MAINWINDOW_H
