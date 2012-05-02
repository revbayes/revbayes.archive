/**
 * @file
 * This file contains the implementation of Move_mscale, which scales
 * a real value with a factor drawn from an appropriate distribution.
 *
 * @brief Implementation of Move_mscale
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

#include "Move_scale.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "StochasticInferenceNode.h"

#include <cmath>
#include <iostream>


/** Constructor for parser */
Move_scale::Move_scale( void ) : InferenceMoveSimple(  ) {
}


/** Clone object */
Move_scale* Move_scale::clone( void ) const {

    return new Move_scale( *this );
}


/** Perform the move */
double Move_scale::performSimpleMove( void ) {
    
//    std::cerr << "Performing scale move on " << node->getName() << std::endl;
    
    // copy all value
    storedValue.value[0] = value.value[0];

    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;

    // Generate new value (no reflection, so we simply abort later if we propose value here outside of support)
    double u = rng->uniform01();
    double newVal = value.value[0] * std::exp( lambda * ( u - 0.5 ) );
    
    // compute the Hastings ratio
    double lnHastingsratio = log( newVal / value.value[0] );

    value.value[0] = newVal;
    
    return lnHastingsratio;
}


void Move_scale::rejectSimpleMove( void ) {
    value.value[0] = storedValue.value[0];
}


void Move_scale::setInternalArguments(const std::vector<StochasticInferenceNode *> &args) {
    
    if ( args.size() > 1 ) {
        throw RbException("Scaling move was initiated with too many DAG nodes.");
    }
    
    const RbValue<void*> v = args[0]->getValue();
    value.value     = static_cast<double *>( v.value );
    value.lengths   = v.lengths;
}


void Move_scale::setAttribute(std::string const &name, const RbValue<void*> &a) {
    
    if ( name == "rate" ) {
        lambda = static_cast<double*>( a.value )[0];
    }
    else if ( name == "weight" ) {
        weight = static_cast<double*>( a.value )[0];
    }
}


