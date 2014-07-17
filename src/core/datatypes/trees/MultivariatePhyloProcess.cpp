/* 
 * File:   MultivariatePhyloProcess.cpp
 * Author: nl
 * 
 * Created on 16 juillet 2014, 20:17
 */

#include "MultivariatePhyloProcess.h"

#include "RbException.h"
#include "RbOptions.h"

#include <cmath>

using namespace RevBayesCore;

// Declarations
std::ostream& operator<<(std::ostream& o, const MultivariatePhyloProcess& x);


/* Default constructor */
MultivariatePhyloProcess::MultivariatePhyloProcess(void) : MatrixReal() {
    
}

/* constructor based on a timetree and a dimension for the process */
MultivariatePhyloProcess::MultivariatePhyloProcess(const TimeTree* intree, size_t indim) : MatrixReal(intree->getNumberOfNodes(), indim, 0.0), tree(intree) {
    
}


/* Copy constructor */
MultivariatePhyloProcess::MultivariatePhyloProcess(const MultivariatePhyloProcess& p) : MatrixReal(p), tree(p.getTimeTree()) {
    
}


/* Destructor */
MultivariatePhyloProcess::~MultivariatePhyloProcess(void) {
}


MultivariatePhyloProcess& MultivariatePhyloProcess::operator=(const MultivariatePhyloProcess &p) {
    
    if (this != &p) {
        MatrixReal::operator=(p);
        tree = p.getTimeTree();
    }
    
    return *this;
}


/* Clone function */
MultivariatePhyloProcess* MultivariatePhyloProcess::clone(void) const {
    
    return new MultivariatePhyloProcess(*this);
}


/*
void MultivariatePhyloProcess::resizeElementVectors(size_t n) {
    // remove all elements
    branchLengths.clear();
    
    // resize to new dimension
    branchLengths.resize( n );
}
*/


double MultivariatePhyloProcess::getRootVal(int k) const {
    
        return (*this)[getTimeTree()->getRoot().getIndex()][k];
}

double MultivariatePhyloProcess::getMean(int k) const {
    
    int n = 0;
    double e1 = 0;
    double e2 = 0;
    recursiveGetStats(k, getTimeTree()->getRoot(), e1, e2, n);
    e1 /= n;
    e2 /= n;
    e2 -= e1 * e1;
    return e1;
}

double MultivariatePhyloProcess::getStdev(int k) const {
    
    int n = 0;
    double e1 = 0;
    double e2 = 0;
    recursiveGetStats(k, getTimeTree()->getRoot(), e1, e2, n);
    e1 /= n;
    e2 /= n;
    e2 -= e1 * e1;
    return sqrt(e2);
}


void MultivariatePhyloProcess::recursiveGetStats(int k, const TopologyNode& from, double& e1, double& e2, int& n) const {

    double tmp = (*this)[from.getIndex()][k];

    n++;
    e1 += tmp;
    e2 += tmp * tmp;
    
    // propagate forward
    size_t numChildren = from.getNumberOfChildren();
    for (size_t i = 0; i < numChildren; ++i) {
        recursiveGetStats(k,from.getChild(i),e1,e2,n);
    }
    
}


/*
std::ostream& operator<<(std::ostream& o, const MultivariatePhyloProcess& x) {
    o << x.getNewickRepresentation();
    
    return o;
}
*/

