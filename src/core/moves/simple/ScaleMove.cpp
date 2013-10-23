/**
 * @file
 * This file contains the implementation of ScaleMove, which scales
 * a real value with a factor drawn from an appropriate distribution.
 *
 * @brief Implementation of ScaleMove
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id$
 */

#include "ScaleMove.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "TypedDagNode.h"

#include <cmath>
#include <iostream>

using namespace RevBayesCore;

/* Constructor */
ScaleMove::ScaleMove( StochasticNode<double> *n, double l, bool t, double w ) : SimpleMove( n, w, t ), variable( n ), storedValue( 0.0 ), lambda( l ) {
    // we need to allocate memory for the stored value
    
}


/* Clone object */
ScaleMove* ScaleMove::clone( void ) const {

    return new ScaleMove( *this );
}



const std::string& ScaleMove::getMoveName( void ) const {
    static std::string name = "Scaling";
    
    return name;
}


/** Perform the move */
double ScaleMove::performSimpleMove( void ) {
        
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    double &val = variable->getValue();
    
    // copy value
    storedValue = val;
    
    // Generate new value (no reflection, so we simply abort later if we propose value here outside of support)
    double u = rng->uniform01();
    double scalingFactor = std::exp( lambda * ( u - 0.5 ) );
    val *= scalingFactor;
    
    // compute the Hastings ratio
    double lnHastingsratio = log( scalingFactor );
    
    return lnHastingsratio;
}


void ScaleMove::printParameterSummary(std::ostream &o) const {
    
    o << "lambda = " << lambda;

}


void ScaleMove::rejectSimpleMove( void ) {
    // swap current value and stored value
    variable->setValue( new double(storedValue) );
    
}


void ScaleMove::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    SimpleMove::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode<double>* >(newN) ;
    
}


void ScaleMove::tune( void ) {
    
    double rate = numAccepted / double(numTried);
    
    if ( rate > 0.44 ) 
    {
        lambda *= (1.0 + ((rate-0.44)/0.56) );
    }
    else 
    {
        lambda /= (2.0 - rate/0.44 );
    }
//  For debugging I assume?? /Fredrik
//    std::cout << lambda << "\n";
    
}

