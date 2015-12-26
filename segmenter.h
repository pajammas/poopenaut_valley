#ifndef SEGMENTER_H
#define SEGMENTER_H

#include <QImage>
#include <Eigen/Core>
#include <Eigen/SparseCore>

using namespace Eigen;


class Segmenter 
{
    public:
        Segmenter(const QImage *);
        Segmenter();
        ~Segmenter();
        QVector<QPoint> segment(const QVector<QPoint> *, const QVector<QPoint> *);

    private:
        const QImage *image;
        // I'm lazy and don't want to call image.height()
        int h, w;
        // Save -1 * beta / sigma, as beta is never used alone.
        double beta, negBetaSigma;

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

int norm(QColor, QColor);

#endif // Segmenter_H
