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
#include "Workspace.h"

#include <cmath>
#include <cassert>


/** Constructor for parser */
Move_mslide::Move_mslide( void ) : MoveSimple( getMemberRules() ), delta( NULL ) {
}


/** Copy constructor */
Move_mslide::Move_mslide(const Move_mslide &ms) : MoveSimple(ms), delta( ms.delta ) {
    
}


/** Clone object */
Move_mslide* Move_mslide::clone( void ) const {

    return new Move_mslide( *this );
}


/** Get class name of object */
const std::string& Move_mslide::getClassName(void) { 
    
    static std::string rbClassName = "Sliding move";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Move_mslide::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( MoveSimple::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Move_mslide::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Return member rules */
const MemberRules& Move_mslide::getMemberRules( void ) const {

    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;

    if ( !rulesSet ) {
        
        memberRules.push_back( new ValueRule( "variable", Real::getClassTypeSpec() ) );

        /* Inherit weight from MoveSimple, put it after variable */
        const MemberRules& inheritedRules = MoveSimple::getMemberRules();
        memberRules.insert( memberRules.end(), inheritedRules.begin(), inheritedRules.end() ); 

        memberRules.push_back( new ValueRule( "delta", RealPos::getClassTypeSpec() ) );

        rulesSet = true;
    }

    return memberRules;
}


/** Return the random variable type appropriate for the move */
const TypeSpec Move_mslide::getVariableType( void ) const {

    return Real::getClassTypeSpec();
}


/** Perform the move */
double Move_mslide::perform( void ) {

    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;

    // Get relevant values
    StochasticNode* nodePtr = static_cast<StochasticNode*>( node->getDagNode() );
    const RealPos& d = static_cast<const RealPos&>( delta->getValue() );

    double curVal  =  static_cast<const Real&>( nodePtr->getValue() ).getValue();
    const Real& min = static_cast<const DistributionContinuous&>( nodePtr->getDistribution() ).getMin();
    const Real& max = static_cast<const DistributionContinuous&>( nodePtr->getDistribution() ).getMax();
    double minVal  = min.getValue();
    double maxVal  = max.getValue();

    Real u      = rng->uniform01();
    Real newVal = curVal + ( d.getValue() * ( u - 0.5 ) );

    /* reflect the new value */
    do {
        if ( newVal < minVal )
            newVal = 2.0 * minVal - newVal;
        else if ( newVal > maxVal )
            newVal = 2.0 * maxVal - newVal;
    } while ( newVal < minVal || newVal > maxVal );

    // FIXME: not the most efficient way of handling multiple reflections :-P

    nodePtr->setValue( newVal.clone() );
	
    return 0.0;
}


/** We catch here the setting of the member variables to store our parameters. */
void Move_mslide::setMemberVariable(std::string const &name, const Variable* var) {
    
    // test whether we want to set the variable 
    if ( name == "delta" ) {
        delta = var;
    }
    else {
        MoveSimple::setMemberVariable(name, var);
    }
}

