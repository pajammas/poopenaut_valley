#ifndef SEGMENTER_H
#define SEGMENTER_H

#include <QImage>
#include <Eigen/Core>
#include <Eigen/SparseCore>

using namespace Eigen;


class Segmenter 
{
    public:
        Segmenter(QImage *);
        Segmenter();
        ~Segmenter();
        QVector<QPoint> segment(QVector<QPoint> *, QVector<QPoint> *);

    private:
        QImage image;
        // I'm lazy and don't want to call image.height()
        int h, w;
        // And it's faster just to compute negBetaSigma once.
        double beta, negBetaSigma;

        // Top-level helper functions
        void setSigma();
        SparseMatrix<double> getLMatrix();
        SparseMatrix<double> getIMatrix(QVector<QPoint> *, QVector<QPoint> *);
        VectorXd getBVector(QVector<QPoint> *, QVector<QPoint> *);
        
        // Basic helper functions.
        QVector<QPoint> neighbors(int, int);
        bool inBounds(int, int);
        double weight(QColor, QColor);
};

int norm(QColor, QColor);

#endif // Segmenter_H
