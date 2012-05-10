/**
 * @file
 * This file contains the implementation of Move_mslide,
 * which moves a real value uniformly within a sliding window.
 *
 * @brief Implementation of Move_mslide
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

#include "DistributionContinuous.h"
#include "Move_slide.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "StochasticInferenceNode.h"

#include <cmath>
#include <cassert>


/** Constructor for parser */
Move_slide::Move_slide( void ) : InferenceMoveSimple(  ) {
}


/** Clone object */
Move_slide* Move_slide::clone( void ) const {

    return new Move_slide( *this );
}


/** Perform the move */
double Move_slide::performSimpleMove( void ) {

    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    // copy all value
    storedValue.value[0] = value.value[0];

    double min = static_cast<const DistributionContinuous&>( nodes[0]->getDistribution() ).getMin();
    double max = static_cast<const DistributionContinuous&>( nodes[0]->getDistribution() ).getMax();

    double u      = rng->uniform01();
    double newVal = value.value[0] + ( delta * ( u - 0.5 ) );

    /* reflect the new value */
    do {
        if ( newVal < min )
            newVal = 2.0 * min - newVal;
        else if ( newVal > max )
            newVal = 2.0 * max - newVal;
    } while ( newVal < min || newVal > max );

    // FIXME: not the most efficient way of handling multiple reflections :-P

    value.value[0] = newVal;
//    nodePtr->setValue( newVal.clone() );
	
    return 0.0;
}


void Move_slide::rejectSimpleMove( void ) {
    
    value.value[0] = storedValue.value[0];
}


void Move_slide::setInternalArguments(const std::vector<StochasticInferenceNode *> &args) {
    
    if ( args.size() > 1 ) {
        throw RbException("Scaling move was initiated with too many DAG nodes.");
    }
    
    const RbValue<void*> v = args[0]->getValue();
    value.value     = static_cast<double *>( v.value );
    value.lengths   = v.lengths;
    
    size_t elements = 1;
    for (size_t i = 0; i < value.lengths.size(); ++i) {
        elements *= value.lengths[i];
    }
    storedValue.value = new double[elements];
    storedValue.lengths = value.lengths;
}


void Move_slide::setAttribute(std::string const &name, const RbValue<void*> &a) {
    
    if ( name == "delta" ) {
        delta = static_cast<double*>( a.value )[0];
    }
    else if ( name == "weight" ) {
        weight = static_cast<double*>( a.value )[0];
    }
}


