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

#include "ConstantNode.h"
#include "Move_mscale.h"
#include "RandomNumberGenerator.h"
#include "RbUtil.h"
#include "RealPos.h"
#include "StochasticNode.h"
#include "ValueRule.h"
#include "VectorString.h"
#include "Workspace.h"

#include <cmath>


/** Constructor for parser */
Move_mscale::Move_mscale( void ) : MoveSimple( getMemberRules() ), lambda( NULL ) {
}


/** Clone object */
Move_mscale* Move_mscale::clone( void ) const {

    return new Move_mscale( *this );
}


/** Get class name of object */
const std::string& Move_mscale::getClassName(void) { 
    
    static std::string rbClassName = "Scaling move";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Move_mscale::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( MoveSimple::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Move_mscale::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Return member rules */
const MemberRules& Move_mscale::getMemberRules( void ) const {

    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;

    if ( !rulesSet ) {
        memberRules.push_back( new ValueRule( "variable", RealPos::getClassTypeSpec() ) );

        /* Inherit weight from MoveSimple, put it after variable */
        const MemberRules& inheritedRules = MoveSimple::getMemberRules();
        memberRules.insert( memberRules.end(), inheritedRules.begin(), inheritedRules.end() ); 

        memberRules.push_back( new ValueRule( "lambda", RealPos::getClassTypeSpec() ) );

        rulesSet = true;
    }

    return memberRules;
}



/** Return the random variable type appropriate for the move */
const TypeSpec Move_mscale::getVariableType( void ) const {

    return RealPos::getClassTypeSpec();
}


/** Perform the move */
double Move_mscale::perform( void ) {

    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;

    // Get relevant values
    StochasticNode*        nodePtr = static_cast<StochasticNode*>( node->getDagNode() );
    const RealPos&         l       = static_cast<const RealPos&>( lambda->getValue() );
    const RealPos&         curVal  = static_cast<const RealPos&>( nodePtr->getValue() );

    // Generate new value (no reflection, so we simply abort later if we propose value here outside of support)
    RealPos u      = rng->uniform01();
    RealPos newVal = curVal * std::exp( l * ( u - 0.5 ) );
    
    // compute the Hastings ratio
    double lnHastingsratio = log( newVal / curVal );

    // Propose new value
    nodePtr->setValue( newVal.clone() );
    
    return lnHastingsratio;
}



/** We catch here the setting of the member variables to store our parameters. */
void Move_mscale::setMemberVariable(std::string const &name, Variable* var) {
    
    // test whether we want to set the variable 
    if ( name == "lambda" ) {
        lambda = var;
    } 
    else {
        MoveSimple::setMemberVariable(name, var);
    }
}

