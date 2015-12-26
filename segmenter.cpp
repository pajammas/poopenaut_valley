#include "segmenter.h"

#include <iostream>
#include <cmath>

#include <QColor>
#include <Eigen/SparseCholesky>

using namespace Eigen;

#define BETA_VALUE 0.005

/* Issues (in approximate order of importance):
 *
 * Output image sometimes takes a LOOONG time to appear.
 * Likely because segment()'s output is a massive QVector.
 *
 * Beta slider, reset button!
 *
 * Slow as fuck. Computing the cholesky solver takes 30 times more time than anything else.
 * Which Cholesky to use? Base, LLT, or LDLT?
 *
 * Doesn't seem mathy to process the rescaled version of image
 *
 * It would be nice to display the original image with seeds alongside the result.
 *
 * Mywidget needs a better name
 *
 * Cancelling file select window prints errors
 * Drawing out of bounds does too
 *
 * Nicer Eigenpath finding?
 */

// For testing:
 #include <time.h>


Segmenter::Segmenter(QImage *imageIn) {
    image = *imageIn;
    h = image.height();
    w = image.width();
    beta = BETA_VALUE;
    setSigma();
    //cout << negBetaSigma;
}

Segmenter::Segmenter() {
    std::cout << "WARNING: Segmenter should be given an image upon construction." << std::endl
         << "The object created with this call will not work." << std::endl;
}

Segmenter::~Segmenter() { }

// This function will find sigma and set negBetaSigma for later use.
void Segmenter::setSigma() {
    // Sigma is the maximum norm of the image.
    double sigma = 0.1;
    
    /* According to paper sigma should be equal to 0.1 ups ^__^
     * I mean good try and well done, and girls will like your for loops
     *  with c and r instead of i,j buuuuuut.... you knoooow....
     *
    
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
    */

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
QVector<QPoint> Segmenter::segment(QVector<QPoint> *fore, QVector<QPoint> *back) {

    clock_t t = clock();
    std::cout << "Starting...\n";
    std::cout << "Time elapsed, in seconds: " << ((float)(clock()-t))/CLOCKS_PER_SEC << std::endl;

    // We should optimize so these aren't all stored at once.
    SparseMatrix<double> L = getLMatrix();

    std::cout << "L computed.\n";
    std::cout << "Time elapsed, in seconds: " << ((float)(clock()-t))/CLOCKS_PER_SEC << std::endl;

    SparseMatrix<double> A = getIMatrix(fore, back) + L*L;

    std::cout << "A computed.\n";
    std::cout << "Time elapsed, in seconds: " << ((float)(clock()-t))/CLOCKS_PER_SEC << std::endl;

    A.makeCompressed();

    VectorXd B = getBVector(fore, back);

    // Apparently there are other options for an Ax=b solver.
    // We could take some time to choose the best.
    // Construct a 'solver' object using A
    SimplicialCholesky< SparseMatrix<double> > solver(A);

    if (solver.info() != Success) {
        std::cout << "Error: couldn't decompose A." << std::endl;
        return QVector<QPoint>();
    }

    std::cout << "Solver created." << std::endl;
    std::cout << "Time elapsed, in seconds: " << ((float)(clock()-t))/CLOCKS_PER_SEC << std::endl;

    VectorXd X = solver.solve(B);
    if (solver.info() != Success) {
        std::cout << "Error: couldn't solve AX=B." << std::endl;
        return QVector<QPoint>();
    }

    std::cout << "Ax = b solved.\n";
    std::cout << "Time elapsed, in seconds: " << ((float)(clock()-t))/CLOCKS_PER_SEC << std::endl;


    QVector<QPoint> final_fore;
    QPoint pix;
    int i;

    for (i=0; i<X.size(); i++) {
        // Integer division will always round the value down (floor).
        pix = QPoint(i%w, i/w);
        //cout << pix.x() << ' ' << pix.y() << ' ' << X[i] << endl;

        if (X[i] > 0)
            final_fore += pix;
    }

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
            pix = image.pixel(c,r);
            neighs = neighbors(c, r);

            // For each neighbor:
            for (n=0; n<neighs.size(); n++) {
                // Get the neighbor's color
                neigh = image.pixel(neighs[n]);
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
SparseMatrix<double> Segmenter::getIMatrix(QVector<QPoint> *fore, QVector<QPoint> *back) {

    SparseMatrix<double> I(h*w, h*w);
    // There will be at most 1 entry per column (it's diagonal)
    I.reserve(VectorXi::Constant(h*w, 1));

    int i, r, c;
          
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
VectorXd Segmenter::getBVector(QVector<QPoint> *fore, QVector<QPoint> *back) {
    VectorXd B(h*w);

    int i, r, c;

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
