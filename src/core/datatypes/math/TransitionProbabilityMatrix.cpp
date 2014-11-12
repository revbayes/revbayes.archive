/**
 * @file
 * This file contains the declaration of TransitionProbabilityMatrix, which is
 * class that holds a matrix of transition probabilities in RevBayes.
 *
 * @brief Implementation of TransitionProbabilityMatrix
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 * @package datatypes
 *
 * $Id$
 */

#include "RbException.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <iomanip>


using namespace RevBayesCore;


/** Construct rate matrix with n states */
TransitionProbabilityMatrix::TransitionProbabilityMatrix(size_t n) : nElements( n*n ) {

    theMatrix = new double[ nElements ];
    for ( size_t i = 0; i < nElements; ++i) 
    {
        theMatrix[i] = 0.0;
    }
    
    numStates = n;
}

/** Construct rate matrix with n states */
TransitionProbabilityMatrix::TransitionProbabilityMatrix( const TransitionProbabilityMatrix &tpm ) : numStates( tpm.numStates ), nElements( tpm.nElements ) {
    
    theMatrix = new double[ nElements ];
    for ( size_t i = 0; i < nElements; ++i) 
    {
        theMatrix[i] = tpm.theMatrix[i];
    }
    
}


TransitionProbabilityMatrix::~TransitionProbabilityMatrix() {

    delete [] theMatrix;

}


/** Construct rate matrix with n states */
TransitionProbabilityMatrix& TransitionProbabilityMatrix::operator=( const TransitionProbabilityMatrix &tpm ) {
    
    if ( this != &tpm ) 
    {
        nElements = tpm.nElements;
        numStates = tpm.numStates;
        
        delete [] theMatrix;
        theMatrix = new double[ nElements ];
        for ( size_t i = 0; i < nElements; ++i) 
        {
            theMatrix[i] = tpm.theMatrix[i];
        }
    }
    
    return *this;
}


/** Index operator (const) */
const double* TransitionProbabilityMatrix::operator[]( const size_t i ) const {

    return theMatrix + i*numStates;
}


/** Index operator */
double* TransitionProbabilityMatrix::operator[]( const size_t i ) {
    
    return theMatrix + i*numStates;
}

TransitionProbabilityMatrix& TransitionProbabilityMatrix::operator*=(const TransitionProbabilityMatrix& B) {
    
    TransitionProbabilityMatrix C(numStates);
    for (size_t i=0; i<numStates; i++)
    {
        for (size_t j=0; j<numStates; j++)
        {
            double sum = 0.0;
            for (size_t k=0; k<numStates; k++)
                sum += (*this)[i][k] * B[k][j];
            C[i][j] = sum;
        }
    }
    
    for (size_t i=0; i<numStates*numStates; i++)
        theMatrix[i] = C.theMatrix[i];
    
	return *this;
}

double TransitionProbabilityMatrix::getElement(size_t i, size_t j) const {
    
    return *(theMatrix + numStates*i + j);
}


double& TransitionProbabilityMatrix::getElement(size_t i, size_t j) {
    
    return *(theMatrix + numStates*i + j);
}


const double* TransitionProbabilityMatrix::getElements( void ) const {
    
    return theMatrix;
}


double* TransitionProbabilityMatrix::getElements( void ) {
    
    return theMatrix;
}


size_t TransitionProbabilityMatrix::getNumberOfStates( void ) const {
    
    return numStates;
}


size_t TransitionProbabilityMatrix::size(void) const {
    
    return nElements;
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const TransitionProbabilityMatrix& x) {
    
    std::streamsize previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
    
    o << "[ ";
    o << std::fixed;
    o << std::setprecision(4);
    
    // print the RbMatrix with each column of equal width and each column centered on the decimal
    for (size_t i=0; i < x.getNumberOfStates(); i++) 
    {
        if (i == 0)
            o << "[ ";
        else 
            o << "  ";
        
        for (size_t j = 0; j < x.getNumberOfStates(); ++j) 
        {
            if (j != 0)
                o << ", ";
            o << x[i][j];
        }
        o <<  " ]";
        
        if (i == x.size()-1)
            o << " ]";
        else 
            o << " ,\n";
        
    }
    
    o.setf(previousFlags);
    o.precision(previousPrecision);
    
    return o;
}


