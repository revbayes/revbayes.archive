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


/*
std::ostream& operator<<(std::ostream& o, const MultivariatePhyloProcess& x) {
    o << x.getNewickRepresentation();
    
    return o;
}
*/
