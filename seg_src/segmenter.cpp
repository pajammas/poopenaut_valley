#include "segmenter.h"

#include <iostream>
#include <cmath>

#include <QColor>
#include <Eigen/SparseCholesky>

using namespace Eigen;

// For testing:
 #include <time.h>


// Standard constructor
Segmenter::Segmenter(const QImage *imageIn) {
    image = imageIn;
    h = image->height();
    w = image->width();
    negBetaSigma = 0;
}

// Default constructor should not be used
Segmenter::Segmenter() {
    std::cout << "WARNING: Segmenter should be passed an image parameter." << std::endl
              << "The object created with this call will not work." << std::endl;
    image = NULL;
    w = 0;
    h = 0;
    negBetaSigma = 0;
}

Segmenter::~Segmenter() { }

// This function returns the vector of points in the foreground.
// All other points lie in the background.
QVector<QPoint> Segmenter::segment(float beta, const QVector<QPoint> *fore, const QVector<QPoint> *back) {

    // We'll use this constant because weight calculations use -beta/sigma, 
    // and the paper recommends a sigma of 0.1
    negBetaSigma = (-1 * beta) / 0.1;

    // Time profiling
    clock_t t = clock();
    std::cout << "Starting...\n";

    // Never store more than one matrix.
    SparseMatrix<double> A = getLMatrix();
    // A = I + L^2
    A = getIMatrix(fore, back) + A*A;
    A.makeCompressed();
    std::cout << "A computed.\n";
    std::cout << "Time elapsed, in seconds: " << ((float)(clock()-t))/CLOCKS_PER_SEC << std::endl;

    VectorXd B = getBVector(fore, back);


    // Apparently there are other options for an Ax=b solver.
    // We could take some time to choose the best.
    // Construct a 'solver' object using A
    SimplicialLLT< SparseMatrix<double> > solver(A);
    std::cout << "Solver created." << std::endl;
    std::cout << "Time elapsed, in seconds: " << ((float)(clock()-t))/CLOCKS_PER_SEC << std::endl;

    if (solver.info() != Success) {
        std::cout << "Error: couldn't decompose A." << std::endl;
        return QVector<QPoint>();
    }

    VectorXd X = solver.solve(B);
    if (solver.info() != Success) {
        std::cout << "Error: couldn't solve AX=B." << std::endl;
        return QVector<QPoint>();
    }


    QVector<QPoint> final_fore;
    QPoint pix;
    int i;

    for (i=0; i<X.size(); i++) {
        // Integer division will always round the value down (floor).
        pix = QPoint(i%w, i/w);
        //cout << pix.x() << ' ' << pix.y() << ' ' << X[i] << endl;

        if (X[i] < 0)
            final_fore += pix;
        // else final_back +=pix
    }

    // We only need to return one set of pixels
    return final_fore;
}


// L = D - W
// D is a diagonal matrix with the weighted valency of each pixel.
// W is a matrix containing the edge weights between all pixels.
SparseMatrix<double> Segmenter::getLMatrix() {

    SparseMatrix<double> L(h*w, h*w);
    //There will be at most 5 entries per column:
    // 4 from W and 1 from D
    L.reserve(VectorXi::Constant(h*w, 5));

    // Iterators for columns, rows, and neighbors
    int c, r, n;
    QVector<QPoint> neighs;
    QColor pix, neigh;
    double this_weight;

    for (c=0; c<w; c++) {
        for (r=0; r<h; r++) {

            // Get the pixel color and its neighbors' coordinates
            pix = image->pixel(c,r);
            neighs = neighbors(c, r);

            // For each neighbor:
            for (n=0; n<neighs.size(); n++) {
                // Get the neighbor's color
                neigh = image->pixel(neighs[n]);
                this_weight = weight(pix, neigh);

                // This part is an element of the W matrix. Negate it.
                // insert() must create a new entry
                L.insert(r*w+c, neighs[n].y()*w + neighs[n].x()) = -1 * this_weight;
            
                // An element of D matrix. Accumulate all the weights of pix.
                // coeffRef can add to a previous entry
                L.coeffRef(r*w+c, r*w+c) += this_weight;
            }
        }
    }
    L.makeCompressed();
    return L;
}

// I is a diagonal matrix which has 1's only for the pixels
// which are members of the foreground or the background.
SparseMatrix<double> Segmenter::getIMatrix(const QVector<QPoint> *fore, const QVector<QPoint> *back) {

    SparseMatrix<double> I(h*w, h*w);
    // There will be at most 1 entry per column (it's diagonal)
    I.reserve(VectorXi::Constant(h*w, 1));

    int i, r, c;

    // Give all seeds a value of 1; else 0
    for(i=0; i < fore->size(); i++) {
        r = (*fore)[i].y();
        c = (*fore)[i].x();
        I.insert(r*w+c, r*w+c) = 1.0;
    }
    for(i=0; i < back->size(); i++) {
        r = (*back)[i].y();
        c = (*back)[i].x();
        I.insert(r*w+c, r*w+c) = 1.0;
    }

    I.makeCompressed();
    return I;
}

// We could get this at the same time as I, for elegancy/speed.
// B tells which seed set, if any, each pixel is part of.
// The choice of 1.0, -1.0 as labels is arbitrary.
VectorXd Segmenter::getBVector(const QVector<QPoint> *fore, const QVector<QPoint> *back) {
    VectorXd B(h*w);

    int i, r, c;

    // Set seed pixels to 1 or -1; else 0
    for(i=0; i<fore->size(); i++) {
        r = (*fore)[i].y();
        c = (*fore)[i].x();
        B[r*w+c] = 1.0;
    }
    for(i=0; i<back->size(); i++) {
        r = (*back)[i].y();
        c = (*back)[i].x();
        B[r*w+c] = -1.0;
    }

    return B;
}

// Helper function to get the neighbors of a pixel
QVector<QPoint> Segmenter::neighbors(int x, int y) {
    QVector<QPoint> n;

/*  // 8-Neighbors: much slower
    int i, j;
    for (i=-1; i<2; i++) {
        for (j=-1; j<2; j++) {
            // Don't add yourself or out of bounds neighbors
            if (!(i==0 && j==0) && inBounds(x+i, y+j)) 
                n += QPoint(x+i, y+j);
        }
    } */

    // Add all neighbor pixels which are in bounds
    if (inBounds(x, y+1))  n += QPoint(x, y+1);
    if (inBounds(x, y-1))  n += QPoint(x, y-1);
    if (inBounds(x+1, y))  n += QPoint(x+1, y);
    if (inBounds(x-1, y))  n += QPoint(x-1, y);
    
    return n;
}

bool Segmenter::inBounds(int x, int y)
    { return ((x>=0) && (y>=0) && (x<w) && (y<h)); }

// Simple equation from the paper.
double Segmenter::weight(QColor i, QColor j)
    { return exp(negBetaSigma * norm(i,j)) + 0.000001; }

// This doesn't even need to be in the class.
// Returns the maximum of the three squared differences.
int norm(QColor i, QColor j) {
    int r = pow(i.red() - j.red(), 2);
    int g = pow(i.green() - j.green(), 2);
    int b = pow(i.blue() - j.blue(), 2);
    return std::max(r, std::max(g, b));
}
