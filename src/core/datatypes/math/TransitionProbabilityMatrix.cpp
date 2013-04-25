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
TransitionProbabilityMatrix::TransitionProbabilityMatrix(size_t n) : theMatrix(n,n,0.0) {

    numStates = n;
}


/** Index operator (const) */
const std::vector<double>& TransitionProbabilityMatrix::operator[]( const size_t i ) const {

//    if ( i >= numStates )
//        throw RbException( "Index to TransitionProbabilityMatrix[][] out of bounds" );
    return theMatrix[i];
}


/** Index operator */
std::vector<double>& TransitionProbabilityMatrix::operator[]( const size_t i ) {

//    if ( i >= numStates )
//        throw RbException( "Index to TransitionProbabilityMatrix[][] out of bounds" );
    return theMatrix[i];
}


std::vector<std::vector<double> >::const_iterator TransitionProbabilityMatrix::begin( void ) const {
    return theMatrix.begin();
}


std::vector<std::vector<double> >::iterator TransitionProbabilityMatrix::begin( void ) {
    return theMatrix.begin();
}


std::vector<std::vector<double> >::const_iterator TransitionProbabilityMatrix::end( void ) const {
    return theMatrix.end();
}


std::vector<std::vector<double> >::iterator TransitionProbabilityMatrix::end( void ) {
    return theMatrix.end();
}


size_t TransitionProbabilityMatrix::getNumberOfStates( void ) const {
    return numStates;
}


size_t TransitionProbabilityMatrix::size(void) const {
    return theMatrix.size();
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const TransitionProbabilityMatrix& x) {
    std::streamsize previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
    
    o << "[ ";
    o << std::fixed;
    o << std::setprecision(4);
    
    // print the RbMatrix with each column of equal width and each column centered on the decimal
    for (size_t i=0; i < x.getNumberOfStates(); i++) {
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


