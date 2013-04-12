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
#include <fstream>
#include <sstream>
#include <string>


using namespace RevBayesCore;

/** Constructor passes member rules and method inits to base class */
TransitionProbabilityMatrix::TransitionProbabilityMatrix(void) : theMatrix(2,2,0.0) {

    numStates = 2;
}


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


size_t TransitionProbabilityMatrix::size(void) const {
    return theMatrix.size();
}


