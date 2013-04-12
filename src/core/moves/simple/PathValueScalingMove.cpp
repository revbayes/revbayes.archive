/**
 * @file
 * This file contains the implementation of a path-resample move,
 * which resamples the path of a random walk.
 *
 * @brief Implementation of PathResampleMove
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
#include "PathValueScalingMove.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbStatisticsHelper.h"
#include "RbUtil.h"

#include <cmath>

using namespace RevBayesCore;

PathValueScalingMove::PathValueScalingMove(StochasticNode<UnivariateFunction> *v, double l, bool t, double weight) : SimpleMove( v, weight, t ), variable( v ), lambda( l ) {
    
}


/** Clone object */
PathValueScalingMove* PathValueScalingMove::clone( void ) const {
    
    return new PathValueScalingMove( *this );
}



const std::string& PathValueScalingMove::getMoveName( void ) const {
    static std::string name = "Single Path-Value-Scaling";
    
    return name;
}



/** Perform the move */
double PathValueScalingMove::performSimpleMove( void ) {
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    // store the value
    StepFunction& f = static_cast<StepFunction &>( variable->getValue() );
    std::vector<double>& vals = f.getPathValues();
    
    // draw the index first
    size_t n = vals.size();
    index = (int)( n*rng->uniform01() );
    
    double &val = vals[index];
    storedValue = val;
    
#ifdef DEBUG_MCMC
    if (index < 0 || index >= vals.size()) {
        throw RbException("Trying to propose new value of path outside specified range.");
    }
#endif
    
    // Generate new value (no reflection, so we simply abort later if we propose value here outside of support)
    double u = rng->uniform01();
    double scalingFactor = std::exp( lambda * ( u - 0.5 ) );
    val *= scalingFactor;
    
    // compute the Hastings ratio
    double lnHastingsratio = log( scalingFactor );
    
    return lnHastingsratio;
}


void PathValueScalingMove::printParameterSummary(std::ostream &o) const {
    o << "lambda = " << lambda;
}


void PathValueScalingMove::rejectSimpleMove( void ) {

    StepFunction& f = static_cast<StepFunction &>( variable->getValue() );
    std::vector<double>& vals = f.getPathValues();
    vals[index] = storedValue;
}


void PathValueScalingMove::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    SimpleMove::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode<UnivariateFunction>* >( newN );
}


void PathValueScalingMove::tune( void ) {
    double rate = numAccepted / double(numTried);
    
    if ( rate > 0.44 ) {
        lambda *= (1.0 + ((rate-0.44)/0.56) );
    }
    else {
        lambda /= (2.0 - rate/0.44 );
    }
}

