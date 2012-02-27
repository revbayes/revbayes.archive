/**
 * @file
 * This file contains the implementation of Move_reallocateAll, which changes
 * the class of all data elements in a mixture.
 *
 * @brief Implementation of Move_reallocateAll
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-02-25 01:17:07 -0800 (Sam, 25 fév 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id: Move_mscale.cpp 1247 2012-02-25 09:17:07Z hoehna $
 */

#include "ConstantNode.h"
#include "Move_reallocateAll.h"
#include "RandomNumberGenerator.h"
#include "RbUtil.h"
#include "RealPos.h"
#include "StochasticNode.h"
#include "ValueRule.h"
#include "VectorString.h"
#include "Workspace.h"
#include "Mixture.h"
//#include "RandomNumberFactory.h"
#include "DistributionMultinomial.h"
//#include "Dist_multinomial.h"


#include <cmath>


/** Constructor for parser */
Move_reallocateAll::Move_reallocateAll( void ) : MoveSimple( getMemberRules() ) {
}


/** Clone object */
Move_reallocateAll* Move_reallocateAll::clone( void ) const {
    
    return new Move_reallocateAll( *this );
}


/** Get class name of object */
const std::string& Move_reallocateAll::getClassName(void) { 
    
    static std::string rbClassName = "Complete reallocation move";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Move_reallocateAll::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( MoveSimple::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Move_reallocateAll::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Return member rules */
const MemberRules& Move_reallocateAll::getMemberRules( void ) const {
    
    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;
    
    if ( !rulesSet ) {
        memberRules.push_back( new ValueRule( "variable", Mixture::getClassTypeSpec() ) );
        
        /* Inherit weight from MoveSimple, put it after variable */
        const MemberRules& inheritedRules = MoveSimple::getMemberRules();
        memberRules.insert( memberRules.end(), inheritedRules.begin(), inheritedRules.end() ); 
        
       // memberRules.push_back( new ValueRule( "lambda", RealPos::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}



/** Return the random variable type appropriate for the move */
const TypeSpec Move_reallocateAll::getVariableType( void ) const {
    
    return RealPos::getClassTypeSpec();
}


/** Perform the move */
double Move_reallocateAll::perform( void ) {
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    // Get relevant values
 //   StochasticNode*        nodePtr = static_cast<StochasticNode*>( node->getDagNode() );
   // const RealPos&         l       = static_cast<const RealPos&>( lambda->getValue() );
  //  const RealPos&         curVal  = static_cast<const RealPos&>( nodePtr->getValue() );
    
    // Generate new allocation vector
    Mixture mix = static_cast< const Mixture&>( mixture->getValue() );
    std::vector<int> newVal = RbStatistics::Multinomial::rv(mix.getClassProbabilities().getValue(), (int) (mix.getNumberOfElements() ), *rng);

    
    // Generate new value (no reflection, so we simply abort later if we propose value here outside of support)
    RealPos u      = rng->uniform01();
 //   RealPos newVal = curVal * std::exp( l * ( u - 0.5 ) );
    
    // compute the Hastings ratio
    double lnHastingsratio = 1.0 ; //log( newVal / curVal );
    
    // Propose new value
  //  nodePtr->setValue( newVal.clone() );
    
    return lnHastingsratio;
}



/** We catch here the setting of the member variables to store our parameters. */
void Move_reallocateAll::setMemberVariable(std::string const &name, Variable* var) {
    
    // test whether we want to set the variable 
    if ( name == "mixture" ) {
        mixture = var;
    } 
    else {
        MoveSimple::setMemberVariable(name, var);
    }
}

