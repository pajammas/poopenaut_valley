#ifndef SEGMENTER_H
#define SEGMENTER_H

#include <QImage>
#include <Eigen/Core>
#include <Eigen/SparseCore>

// This is the math class, so this will make things compact
using namespace Eigen;


class Segmenter 
{
    public:
        // Constructors and destructor
        Segmenter(const QImage *);
        Segmenter();
        ~Segmenter();
        QVector<QPoint> segment(float, const QVector<QPoint> *, const QVector<QPoint> *);

    private:
        // Save a reference to the image loaded in mainwindow
        const QImage *image;
        // Save these for compactness
        int h, w;
        // Don't want to re-calculate this all the time
        float negBetaSigma;

        // Top-level helper functions
        void setSigma();
        SparseMatrix<double> getLMatrix();
        SparseMatrix<double> getIMatrix(const QVector<QPoint> *, const QVector<QPoint> *);
        VectorXd getBVector(const QVector<QPoint> *, const QVector<QPoint> *);
        
        // Basic helper functions.
        QVector<QPoint> neighbors(int, int);
        bool inBounds(int, int);
        double weight(QColor, QColor);
};

// This doesn't even have to be a class member!!
int norm(QColor, QColor);

#endif // Segmenter_H
