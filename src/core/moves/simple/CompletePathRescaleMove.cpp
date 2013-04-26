/**
 * @file
 * This file contains the implementation of a complete path-rescale move,
 * which rescales the path of a random walk.
 *
 * @brief Implementation of CompletePathRescalseMove
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-05-11 14:54:35 +0200 (Fri, 11 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-08-14, version 1.0
 *
 * $Id: SimplexMove.cpp 1535 2012-05-11 12:54:35Z hoehna $
 */

#include "StepFunction.h"
#include "CompletePathRescaleMove.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbStatisticsHelper.h"
#include "RbUtil.h"

#include <cmath>

using namespace RevBayesCore;

CompletePathRescaleMove::CompletePathRescaleMove(StochasticNode<UnivariateFunction> *v, double l, bool t, double weight) : SimpleMove( v, weight, t ), variable( v ), lambda( l ) {
    
}


/** Clone object */
CompletePathRescaleMove* CompletePathRescaleMove::clone( void ) const {
    
    return new CompletePathRescaleMove( *this );
}



const std::string& CompletePathRescaleMove::getMoveName( void ) const {
    static std::string name = "Complete-Path-Rescale";
    
    return name;
}



/** Perform the move */
double CompletePathRescaleMove::performSimpleMove( void ) {
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    // store the value
    StepFunction& f = static_cast<StepFunction &>( variable->getValue() );
    std::vector<double>& vals = f.getPathValues();
    storedValue = vals;
    
    // get the number of intervals
    size_t n = vals.size();
    
    // Generate new value (no reflection, so we simply abort later if we propose value here outside of support)
    double u = rng->uniform01();
    double scalingFactor = std::exp( lambda * ( u - 0.5 ) );
    for (size_t i = 0; i < n; ++i) {
        vals[i] *= scalingFactor;
    }
    
    // compute the Hastings ratio
    double lnHastingsratio = (log( scalingFactor )*n);
    
    return lnHastingsratio;
}


void CompletePathRescaleMove::printParameterSummary(std::ostream &o) const {
    o << "lambda = " << lambda;
}


void CompletePathRescaleMove::rejectSimpleMove( void ) {
    StepFunction& f = static_cast<StepFunction &>( variable->getValue() );
    std::vector<double>& vals = f.getPathValues();
    vals = storedValue;
}


void CompletePathRescaleMove::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    SimpleMove::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode<UnivariateFunction>* >( newN );
}


void CompletePathRescaleMove::tune( void ) {
    double rate = numAccepted / double(numTried);
    
    if ( rate > 0.234 ) {
        lambda *= (1.0 + ((rate-0.234)/0.766) );
    }
    else {
        lambda /= (2.0 - rate/0.234 );
    }
}

