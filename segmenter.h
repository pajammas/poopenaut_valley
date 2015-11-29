#ifndef SEGMENTER_H
#define SEGMENTER_H

#include <QImage>
#include <Eigen/Core>

using namespace Eigen;


class Segmenter 
{
    public:
        explicit Segmenter(QImage);
        ~Segmenter();
        QVector<QPoint> segment(QVector<QPoint>, QVector<QPoint>);

    private:
        QImage image;
        // I'm lazy and don't want to call image.height()
        int h, w;
        // And it's faster just to compute negBetaSigma once.
        float beta, negBetaSigma;

        // Top-level helper functions
        void setSigma();
        MatrixXf getLMatrix();
        MatrixXf getIMatrix(QVector<QPoint>, QVector<QPoint>);
        VectorXf getBVector(QVector<QPoint>, QVector<QPoint>);
        
        // Basic helper functions.
        QVector<QPoint> neighbors(int, int);
        bool inBounds(int, int);
        float weight(QColor, QColor);
};

int norm(QColor, QColor);

#endif // Segmenter_H
