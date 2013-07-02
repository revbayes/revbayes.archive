/**
 * @file
 * This file contains the implementation of SingleElementScaleMove, which scales
 * a real value with a factor drawn from an appropriate distribution.
 *
 * @brief Implementation of SingleElementScaleMove
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-11-29 14:02:28 -0800 (Thu, 29 Nov 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id: SingleElementScaleMove.cpp 1877 2012-11-29 22:02:28Z hoehna $
 */

#include "SingleElementScaleMove.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbMathVector.h"
#include "TypedDagNode.h"

#include <cmath>
#include <iostream>

using namespace RevBayesCore;

/* Constructor */
SingleElementScaleMove::SingleElementScaleMove( StochasticNode< std::vector<double> > *n, double l, bool t, double w ) : SimpleMove( n, w, t ), variable( n ), storedValue( 0.0 ), lambda( l ) {
    // we need to allocate memory for the stored value
    
}


/* Clone object */
SingleElementScaleMove* SingleElementScaleMove::clone( void ) const {
    
    return new SingleElementScaleMove( *this );
}



const std::string& SingleElementScaleMove::getMoveName( void ) const {
    static std::string name = "Scaling";
    
    return name;
}


/** Perform the move */
double SingleElementScaleMove::performSimpleMove( void ) {
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    std::vector<double> &val = variable->getValue();
    
    // choose an index
    index = int(rng->uniform01() * val.size());
    
    // copy value
    storedValue = val;
    
    // Generate new value (no reflection, so we simply abort later if we propose value here outside of support)
    double u = rng->uniform01();
    double scalingFactor = std::exp( lambda * ( u - 0.5 ) );
    val[index] *= scalingFactor;
	
	// normalize 
	RbMath::normalize ( val, 1.0);
    
    // compute the Hastings ratio
    double lnHastingsratio = log( scalingFactor );
    
    return lnHastingsratio;
}


void SingleElementScaleMove::printParameterSummary(std::ostream &o) const {
    o << "lambda = " << lambda;
}


void SingleElementScaleMove::rejectSimpleMove( void ) {
    
    std::vector<double> &val = variable->getValue();
	
    // swap current value and stored value
    val = storedValue;
	
}


void SingleElementScaleMove::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    SimpleMove::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode< std::vector<double> >* >(newN) ;
}


void SingleElementScaleMove::tune( void ) {
    double rate = numAccepted / double(numTried);
    
    if ( rate > 0.44 ) {
        lambda *= (1.0 + ((rate-0.44)/0.56) );
    }
    else {
        lambda /= (2.0 - rate/0.44 );
    }
}

