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

#include "Move_slide.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"

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

    // Get relevant values
//    StochasticNode* nodePtr = node;
//    const RealPos& d = static_cast<const RealPos&>( delta->getValue() );
//
//    double curVal  =  static_cast<const Real&>( nodePtr->getValue() ).getValue();
//    const Real& min = static_cast<const ParserDistributionContinuous&>( nodePtr->getDistribution() ).getMin();
//    const Real& max = static_cast<const ParserDistributionContinuous&>( nodePtr->getDistribution() ).getMax();
//    double minVal  = min.getValue();
//    double maxVal  = max.getValue();
//
//    Real u      = rng->uniform01();
//    Real newVal = curVal + ( d.getValue() * ( u - 0.5 ) );
//
//    /* reflect the new value */
//    do {
//        if ( newVal < minVal )
//            newVal = 2.0 * minVal - newVal;
//        else if ( newVal > maxVal )
//            newVal = 2.0 * maxVal - newVal;
//    } while ( newVal < minVal || newVal > maxVal );
//
//    // FIXME: not the most efficient way of handling multiple reflections :-P
//
//    nodePtr->setValue( newVal.clone() );
	
    return 0.0;
}


void Move_slide::rejectSimpleMove( void ) {
    
}


void Move_slide::setArguments(const std::vector<InferenceDagNode *> &args) {
    
}


