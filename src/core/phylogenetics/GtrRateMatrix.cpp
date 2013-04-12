//
//  GtrRateMatrix.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 12/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "GtrRateMatrix.h"
#include "RateMatrix_GTR.h"
#include "RbException.h"
#include "RbMathMatrix.h"

using namespace RevBayesCore;

RateMatrix_GTR RbPhylogenetics::Gtr::computeRateMatrix(const std::vector<double> &er, const std::vector<double> &bf) {
    RateMatrix_GTR rm = RateMatrix_GTR( bf.size() );
    computeRateMatrix(er, bf, &rm);
    
    return rm;
}


void RbPhylogenetics::Gtr::computeRateMatrix(const std::vector<double> &r, const std::vector<double> &f, RateMatrix_GTR *rm) {
    
    // initialize the number of states
    size_t nStates = rm->getNumberOfStates();
    
    // check the sizes of the simplices, to make certain that they are consistent
    // with a model with nStates states
    if ( f.size() != nStates ) {
        std::stringstream o;
        o << "The simplex containing the state frequencies is not of size ";
        o << nStates;
        throw( RbException(o.str()) );
    }
    if (r.size() != nStates*(nStates-1)/2) {
        std::stringstream o;
        o << "The simplex containing the rates is not of size (";
        o << nStates;
        o << " choose 2) = ";
        o << nStates*(nStates-1)/2;
        throw( RbException(o.str()) );
    }
    
    RateMatrix_GTR& m = *rm;
    
    // set the off-diagonal portions of the rate matrix
    for (size_t i=0, k=0; i<nStates; i++) {
        for (size_t j=i+1; j<nStates; j++) {
            m[i][j] = r[k] * f[j];
            m[j][i] = r[k] * f[i];
            k++;
        }
    }
    
    // set the diagonal elements of the rate matrix
    m.setDiagonal();
    
    // Set the stationary frequencies for the rate matrix. Note that we
    // can do this in two ways. First, we can call calculateStationaryFrequencies
    // on the RateMatrix object. This function call calculates the stationary
    // frequencies using only knowledge of the rate matrix. Second, we can set
    // the stationary frequencies directly. This is what we do here, because the
    // stationary frequencies have been build directly into the rate matrix.
    m.setStationaryFrequencies( f );
    
    // rescale the rate matrix such that the average rate is 1.0
    m.rescaleToAverageRate(1.0);
    
    // we know that the GTR model is time reversible (just look at the name of the
    // model!), so we might as well set its reversibility flag directly
    m.setTimeReversible(true);
    
    // Now that we have set the rate matrix, we should update its eigen system
    m.updateEigenSystem();
}

