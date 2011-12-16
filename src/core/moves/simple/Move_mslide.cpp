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
#include "RbUtil.h"
#include "RealPos.h"
#include "StochasticNode.h"
#include "ValueRule.h"
#include "VectorString.h"
#include "Workspace.h"

#include <cmath>
#include <cassert>


// Definition of the static type spec member
const TypeSpec Move_mslide::typeSpec(Move_mslide_name);

/** Constructor for parser */
Move_mslide::Move_mslide( void ) : MoveSimple( getMemberRules() ) {
}


/** Copy constructor */
Move_mslide::Move_mslide(const Move_mslide &ms) : MoveSimple(ms) {
    
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
RbPtr<const MemberRules> Move_mslide::getMemberRules( void ) const {

    static RbPtr<MemberRules> memberRules( new MemberRules() );
    static bool        rulesSet = false;

    if ( !rulesSet ) {
        
        TypeSpec varType( Real_name );
        memberRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "variable", varType ) ) );

        /* Inherit weight from MoveSimple, put it after variable */
        RbPtr<const MemberRules> inheritedRules = MoveSimple::getMemberRules();
        memberRules->insert( memberRules->end(), inheritedRules->begin(), inheritedRules->end() ); 

        memberRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "delta", RealPos_name ) ) );

        rulesSet = true;
    }

    return RbPtr<const MemberRules>( memberRules );
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Move_mslide::getTypeSpec(void) const {
    return typeSpec;
}


/** Return the random variable type appropriate for the move */
const TypeSpec Move_mslide::getVariableType( void ) const {

    return TypeSpec( Real_name );
}


/** Perform the move */
double Move_mslide::perform( void ) {

    // Get random number generator    
    RbPtr<RandomNumberGenerator> rng     = GLOBAL_RNG;

    // Get relevant values
    RbPtr<StochasticNode> nodePtr( static_cast<StochasticNode*>( (DAGNode*)nodes[0] ) );
    RbPtr<const RealPos> delta( static_cast<const RealPos*>( (const RbObject*)getMemberValue("delta") ) );

    double curVal  =  ( static_cast<const Real*>( (const RbObject*)nodePtr->getValue() ) )->getValue();
    RbPtr<const Real> minPtr = static_cast<const DistributionContinuous*>( (const Distribution*)nodePtr->getDistribution() )->getMin();
    RbPtr<const Real> maxPtr = static_cast<const DistributionContinuous*>( (const Distribution*)nodePtr->getDistribution() )->getMax();
    double minVal  = minPtr->getValue();
    double maxVal  = maxPtr->getValue();

    Real u      = rng->uniform01();
    Real newVal = curVal + ( delta->getValue() * ( u - 0.5 ) );

    /* reflect the new value */
    do {
        if ( newVal < minVal )
            newVal = 2.0 * minVal - newVal;
        else if ( newVal > maxVal )
            newVal = 2.0 * maxVal - newVal;
    } while ( newVal < minVal || newVal > maxVal );

    // FIXME: not the most efficient way of handling multiple reflections :-P

    nodePtr->setValue( RbPtr<RbLanguageObject>( newVal.clone() ) );
	
    return 0.0;
}

