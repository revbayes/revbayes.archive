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

#include "ConstantNode.h"
#include "DistributionContinuous.h"
#include "Move_mslide.h"
#include "RandomNumberGenerator.h"
#include "RbNames.h"
#include "RealPos.h"
#include "StochasticNode.h"
#include "StochasticReferenceRule.h"
#include "ValueRule.h"
#include "VectorString.h"
#include "Workspace.h"

#include <cmath>


/** Constructor for parser */
Move_mslide::Move_mslide( void )
    : MoveSimple( getMemberRules() ) {
}


/** Constructor for internal use */
Move_mslide::Move_mslide( StochasticNode* node, double delta, double weight )
    : MoveSimple( getMemberRules() ) {

    setVariable( "variable", node );
    setValue(    "weight",   new RealPos(weight) );
    setValue(    "delta",    new RealPos( delta  ) );
}


/** Clone object */
Move_mslide* Move_mslide::clone( void ) const {

    return new Move_mslide( *this );
}


/** Get class vector describing type of object */
const VectorString& Move_mslide::getClass() const {

    static VectorString rbClass = VectorString( Move_mslide_name ) + MoveSimple::getClass();
    return rbClass;
}


/** Return member rules */
const MemberRules& Move_mslide::getMemberRules( void ) const {

    static MemberRules memberRules;
    static bool        rulesSet = false;

    if ( !rulesSet ) {

        memberRules.push_back( new StochasticReferenceRule( "variable", Real_name ) );

        /* Inherit weight from MoveSimple, put it after variable */
        const MemberRules& inheritedRules = MoveSimple::getMemberRules();
        memberRules.insert( memberRules.end(), inheritedRules.begin(), inheritedRules.end() ); 

        memberRules.push_back( new ValueRule( "delta", RealPos_name ) );

        rulesSet = true;
    }

    return memberRules;
}


/** Return the random variable type appropriate for the move */
const TypeSpec Move_mslide::getVariableType( void ) const {

    return TypeSpec( Real_name );
}


/** Perform the move */
double Move_mslide::perform( std::set<StochasticNode*>& affectedNodes ) {

    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;

    // Get relevant values
    StochasticNode*         nodePtr =    static_cast<StochasticNode*>( members["variable"].getReference() );
    const RealPos           delta   = *( static_cast<const RealPos*>( getValue("delta")   ) );

    const Real              curVal  = *( static_cast<const Real*                  >( nodePtr->getValue() ) );
    const RbObject*         minPtr  =    static_cast<const DistributionContinuous*>( nodePtr->getDistribution() )->getMin();
    const RbObject*         maxPtr  =    static_cast<const DistributionContinuous*>( nodePtr->getDistribution() )->getMax();
    const Real              minVal  = *( static_cast<const Real*                  >( minPtr ) );
    const Real              maxVal  = *( static_cast<const Real*                  >( maxPtr ) );

    Real u      = rng->uniform01();
    Real newVal = curVal + ( delta * ( u - 0.5 ) );

    /* reflect the new value */
    do {
        if ( newVal < minVal )
            newVal = 2.0 * minVal - newVal;
        else if ( newVal > maxVal )
            newVal = 2.0 * maxVal - newVal;
    } while ( newVal < minVal || newVal > maxVal );

    nodePtr->setValue( newVal.clone(), affectedNodes );
	
    return 0.0;
}

