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
        QVector<QPoint> segment(float, const QVector<QPoint> *, const QVector<QPoint> *);

    private:
        const QImage *image;
        int h, w;
        float negBetaSigma;
        SparseMatrix<double> L_squared;

        void setSigma();
        SparseMatrix<double> getLMatrix();
        SparseMatrix<double> getIMatrix(const QVector<QPoint> *, const QVector<QPoint> *);
        VectorXd getBVector(const QVector<QPoint> *, const QVector<QPoint> *);
        
        QVector<QPoint> neighbors(int, int);
        bool inBounds(int, int);
        double weight(QColor, QColor);
};

int norm(QColor, QColor);

#endif // Segmenter_H
