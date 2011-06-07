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
#include "DistributionInterval.h"
#include "Move_mslide.h"
#include "RandomNumberGenerator.h"
#include "RbNames.h"
#include "RealPos.h"
#include "StochasticNode.h"
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

    setValue( "delta" , new RealPos( delta  ) );
    setValue( "weight", new RealPos( weight ) );

    setNodePtr( node );
}


/** Clone object */
Move_mslide* Move_mslide::clone( void ) const {

    return new Move_mslide( *this );
}


/** Get class vector describing type of object */
const VectorString& Move_mslide::getClass() const {

    static VectorString rbClass = VectorString( Move_mslide_name ) + Move::getClass();
    return rbClass;
}


/** Return member rules */
const MemberRules& Move_mslide::getMemberRules( void ) const {

    static MemberRules memberRules;
    static bool        rulesSet = false;

    if ( !rulesSet ) {

        memberRules.push_back( new ValueRule( "delta", RealPos_name ) );

        /* Inherit weight and rng from Move, put it at back */
        const MemberRules& inheritedRules = Move::getMemberRules();
        memberRules.insert( memberRules.end(), inheritedRules.begin(), inheritedRules.end() );

        rulesSet = true;
    }

    return memberRules;
}


/** Perform the move */
double Move_mslide::perform( std::set<StochasticNode*>& affectedNodes ) {

    // Get affected nodes
    nodePtr->getAffected( affectedNodes );

    // Get relevant values
    const Real              curVal = *( static_cast<const Real*   >( nodePtr->getValue() ) );
    RandomNumberGenerator*  rng    = GLOBAL_RNG;
    const RealPos           delta  = *( static_cast<const RealPos*>( getValue("delta")   ) );
    const RbObject*         min    = static_cast<const DistributionInterval*>( nodePtr->getDistribution() )->getMin();
    const RbObject*         max    = static_cast<const DistributionInterval*>( nodePtr->getDistribution() )->getMax();
    const Real              minVal = *( static_cast<const Real*                >( min ) );
    const Real              maxVal = *( static_cast<const Real*                >( max ) );

    Real u      = rng->uniform01();
    Real newVal = curVal + ( delta * ( u - 0.5 ) );

    /* reflect the new value */
    do {
        if ( newVal < minVal )
            newVal = 2.0 * minVal - newVal;
        else if ( newVal > maxVal )
            newVal = 2.0 * maxVal - newVal;
    } while ( newVal < minVal || newVal > maxVal );

    nodePtr->setValue( newVal.clone() );
	
    return 0.0;
}


