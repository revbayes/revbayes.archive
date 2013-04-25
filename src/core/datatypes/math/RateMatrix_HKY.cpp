/**
 * @file
 * This file contains the implementation of RateMatrix_HKY, which is
 * class that holds a rate matrix in RevBayes.
 *
 * @brief Implementation of RateMatrix_HKY
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-12-11 14:46:24 +0100 (Tue, 11 Dec 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 * @interface Mcmc
 * @package distributions
 *
 * $Id: RateMatrix_HKY.cpp 1921 2012-12-11 13:46:24Z hoehna $
 */

#include "EigenSystem.h"
#include "MatrixComplex.h"
#include "MatrixReal.h"
#include "RateMatrix_HKY.h"
#include "RbException.h"
#include "RbMathMatrix.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace RevBayesCore;

/** Construct rate matrix with n states */
RateMatrix_HKY::RateMatrix_HKY(void) : RateMatrix( 4 ){
    
    kappa = 1.0;
}


/** Copy constructor */
RateMatrix_HKY::RateMatrix_HKY(const RateMatrix_HKY& m) : RateMatrix( m ) {
    kappa = m.kappa;
}


/** Destructor */
RateMatrix_HKY::~RateMatrix_HKY(void) {
    
}


RateMatrix_HKY& RateMatrix_HKY::operator=(const RateMatrix_HKY &r) {
    
    if (this != &r) {
        RateMatrix::operator=( r );
        
        kappa = r.kappa;
    }
    
    return *this;
}



/** Calculate the transition probabilities */
void RateMatrix_HKY::calculateTransitionProbabilities(double t, TransitionProbabilityMatrix& P) const {
    
    // notation:
    double pi_A = theStationaryFreqs[0];
    double pi_C = theStationaryFreqs[1];
    double pi_G = theStationaryFreqs[2];
    double pi_T = theStationaryFreqs[3];
    
    // compute auxilliary variables
    double pi_AG = pi_A + pi_G;
    double pi_CT = pi_C + pi_T;
    
    // compute beta
    double beta = 1.0 / (2.0*pi_AG*pi_CT+2.0*kappa*((pi_A*pi_G)+(pi_C*pi_T))); 
    
    // calculate the transition probabilities
    
    double xx = - beta * t;
    double aa = exp(xx);
    double bbR = exp( (1.0+pi_AG*(kappa-1.0))* xx);
    double bbY = exp( (1.0+pi_CT*(kappa-1.0))* xx);
    double oneminusa = 1 - aa;
    
    P[0][0] = (pi_A * ( pi_AG+pi_CT*aa ) + pi_G*bbR) / pi_AG;
    P[0][1] = pi_C * oneminusa;
    P[0][2] = (pi_G * ( pi_AG+pi_CT*aa ) - pi_G*bbR) / pi_AG;
    P[0][3] = pi_T * oneminusa;
    
    P[1][0] = pi_A * oneminusa;
    P[1][1] = (pi_C * ( pi_CT+pi_AG*aa ) + pi_T*bbY) / pi_CT;
    P[1][2] = pi_G * oneminusa;
    P[1][3] = (pi_T * ( pi_CT-pi_AG*aa ) + pi_T*bbY) / pi_CT;
    
    P[2][0] = (pi_A * ( pi_AG+pi_CT*aa ) - pi_A*bbR) / pi_AG;
    P[2][1] = P[0][1];
    P[2][2] = (pi_G * ( pi_AG+pi_CT*aa ) + pi_A*bbR) / pi_AG;
    P[2][3] = P[0][3];
    
    P[3][0] = P[1][0];
    P[3][1] = (pi_C * ( pi_CT+pi_AG*aa ) - pi_C*bbY) / pi_CT;
    P[3][2] = P[1][2];
    P[3][3] = (pi_T * ( pi_CT-pi_AG*aa ) + pi_C*bbY) / pi_CT;
}


RateMatrix_HKY* RateMatrix_HKY::clone( void ) const {
    return new RateMatrix_HKY( *this );
}


void RateMatrix_HKY::setKappa( double k ) {
    kappa = k;
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const RateMatrix_HKY& x) {
    std::streamsize previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
    
    o << "[ ";
    o << std::fixed;
    o << std::setprecision(4);
    
    // print the RbMatrix with each column of equal width and each column centered on the decimal
    for (size_t i=0; i < x.size(); i++) {
        if (i == 0)
            o << "[ ";
        else 
            o << "  ";
        
        for (size_t j = 0; j < x.size(); ++j) 
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

