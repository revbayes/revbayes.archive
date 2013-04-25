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
    
    rm->updateMatrix();
}

