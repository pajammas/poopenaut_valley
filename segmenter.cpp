#include "segmenter.h"

#include <iostream>
#include <cmath>

#include <QColor>
#include <Eigen/QR>

using namespace std;
using namespace Eigen;


#define BETA_VALUE 1.0


Segmenter::Segmenter(QString fileName) {
    image.load(fileName);
    h = image.height();
    w = image.width();
    beta = BETA_VALUE;
    setSigma();
    //cout << negBetaSigma;
}

Segmenter::~Segmenter() {}


// This function will find sigma and set negBetaSigma for later use.
void Segmenter::setSigma() {
    // Sigma is the maximum norm of the image.
    int sigma = 0;

    // Iterators for rows, columns, and neighbors
    int r, c, n;
    QColor pix, neigh;
    QVector<QPoint> neighs;

    // For each pixel in the image:
    for (c=0; c<w; c++) {
        for (r=0; r<h; r++) {

            // Get the pixel color and its neighbors' coordinates
            pix = image.pixel(c,r);
            neighs = neighbors(c, r);

            // For each neighbor:
            for (n=0; n<neighs.size(); n++) {
                // Get the neighbor's color
                neigh = image.pixel( neighs[n] );
                
                // If the norm is greater, set it as the new max.
                sigma = max(sigma, norm(pix, neigh));
            }
        }
    }
    // If sigma == 0, the image is all the same color.
    // The weight values don't matter as long as they're all equal.
    if (sigma == 0)
        negBetaSigma = 0;
    // Plus, we never divide by zero.
    else
        negBetaSigma = (-1 * beta) / sigma;
}

// This function returns the vector of points in the foreground.
// All other points lie in the background.
QVector<QPoint> Segmenter::segment(QVector<QPoint> fore, QVector<QPoint> back) {

    MatrixXf L = getLMatrix();
    MatrixXf I = getIMatrix(fore, back);
    VectorXf B = getBVector(fore, back);
    MatrixXf A = I + L*L;

    // Apparently there are other options for an Ax=b solver.
    // We could take some time to choose the best.
    VectorXf X = A.colPivHouseholderQr().solve(B);
    //cout << X << endl;

    QVector<QPoint> final_fore;
    QPoint pix;
    int i;
    
    for (i=0; i<X.size(); i++) {
        // Integer division will always round the value down (floor).
        pix = QPoint(i/w, i%w);
        cout << pix.x() << ' ' << pix.y() << ' ' << X[i] << endl;

        if (X[i] > 0)
            final_fore += pix;
    }

    return final_fore;
}


// L = D - W
// D is a diagonal matrix with the weighted valency of each pixel.
// W is a matrix containing the edge weights between all pixels.
MatrixXf Segmenter::getLMatrix() {
    // Dynamic initialization of square matrix, N = rows*cols
    MatrixXf L = MatrixXf::Zero(h*w, h*w);

    // Iterators for columns, rows, and neighbors
    int c, r, n;
    QColor pix, neigh;
    QVector<QPoint> neighs;

    for (c=0; c<w; c++) {
        for (r=0; r<h; r++) {

            // Get the pixel color and its neighbors' coordinates
            pix = image.pixel(c,r);
            neighs = neighbors(c, r);

            // For each neighbor:
            for (n=0; n<neighs.size(); n++) {
                // Get the neighbor's color
                neigh = image.pixel(neighs[n]);

                // This part is an element of the W matrix. Negate it.
                L(r*w+c, neighs[n].y()*w + neighs[n].x()) -= weight(pix, neigh);
                // An element of D matrix. Accumulate all the weights of pix.
                L(r*w+c, r*w+c) += weight(pix, neigh);
                // To avoid more calls to weight(), we could use the W entry.
            }
        }
    }
    return L;
}

// I is a diagonal matrix which has 1's only for the pixels
// which are members of the foreground or the background.
MatrixXf Segmenter::getIMatrix(QVector<QPoint> fore, QVector<QPoint> back) {
    MatrixXf I = MatrixXf::Zero(w*h, w*h);

    int i, r, c;

    for(i=0; i<fore.size(); i++) {
        r = fore[i].y();
        c = fore[i].x();
        I(r*w+c, r*w+c) = 1.0;
    }
    for(i=0; i<back.size(); i++) {
        r = back[i].y();
        c = back[i].x();
        I(r*w+c, r*w+c) = 1.0;
    }

    return I;
}

// We could get this at the same time as I, for elegancy/speed.
// B tells which seed set, if any, each pixel is part of.
// The choice of 1.0, -1.0 as labels is arbitrary.
VectorXf Segmenter::getBVector(QVector<QPoint> fore, QVector<QPoint> back) {
    VectorXf B = VectorXf::Zero(h*w);

    int i, r, c;

    for(i=0; i<fore.size(); i++) {
        r = fore[i].y();
        c = fore[i].x();
        B(r*w+c) = 1.0;
    }
    for(i=0; i<back.size(); i++) {
        r = back[i].y();
        c = back[i].x();
        B(r*w+c) = -1.0;
    }

    return B;
}


QVector<QPoint> Segmenter::neighbors(int x, int y) {
    QVector<QPoint> n;
    // Add all neighbor pixels which are in bounds
    if (inBounds(x, y+1))  n += QPoint(x, y+1);
    if (inBounds(x, y-1))  n += QPoint(x, y-1);
    if (inBounds(x+1, y))  n += QPoint(x+1, y);
    if (inBounds(x-1, y))  n += QPoint(x-1, y);

    // TEST CASES
    /*
    neighbors(0, image.height());
    neighbors(image.width(), 0);
    neighbors(image.width(), image.height());
    cout << "kay\n";
    neighbors(0, image.height()-1);
    neighbors(image.width()-1, 0);
    neighbors(image.width()-1, image.height()-1);
    */

    // For testing
    //int i;
    //for (i=0; i<n.size(); i++) cout << n[i].x() << ' ' << n[i].y() <<'\n';
    return n;
}

bool Segmenter::inBounds(int x, int y)
    { return ((x>=0) && (y>=0) && (x<w) && (y<h)); }

// Simple equation from the paper.1li
float Segmenter::weight(QColor i, QColor j)
    { return exp(negBetaSigma * norm(i,j)) + 0.000001; }

// This doesn't even need to be in the class.
// Returns the maximum of the three squared differences.
int norm(QColor i, QColor j) {
    int r = pow(i.red() - j.red(), 2);
    int g = pow(i.green() - j.green(), 2);
    int b = pow(i.blue() - j.blue(), 2);
    return max(r, max(g, b));
}
