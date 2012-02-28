/**
 * @file
 * This file contains the implementation of Move_reallocate, which changes
 * the class of one data element in a mixture.
 *
 * @brief Implementation of Move_reallocate
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
#include "Move_reallocate.h"
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
Move_reallocate::Move_reallocate( void ) : MoveSimple( getMemberRules() ) {
}


/** Clone object */
Move_reallocate* Move_reallocate::clone( void ) const {
    
    return new Move_reallocate( *this );
}


/** Get class name of object */
const std::string& Move_reallocate::getClassName(void) { 
    
    static std::string rbClassName = "Reallocation of a data element move";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Move_reallocate::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( MoveSimple::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Move_reallocate::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Return member rules */
const MemberRules& Move_reallocate::getMemberRules( void ) const {
    
    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;
    
    if ( !rulesSet ) {
        memberRules.push_back( new ValueRule( "mixture", Mixture::getClassTypeSpec() ) );
        
        /* Inherit weight from MoveSimple, put it after variable */
        const MemberRules& inheritedRules = MoveSimple::getMemberRules();
        memberRules.insert( memberRules.end(), inheritedRules.begin(), inheritedRules.end() ); 
        
        memberRules.push_back( new ValueRule( "index", Natural::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}



/** Return the random variable type appropriate for the move */
const TypeSpec Move_reallocate::getVariableType( void ) const {
    
    return RealPos::getClassTypeSpec();
}


/** Perform the move */
double Move_reallocate::perform( void ) {
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    // Get relevant values
    //   StochasticNode*        nodePtr = static_cast<StochasticNode*>( node->getDagNode() );
    // const RealPos&         l       = static_cast<const RealPos&>( lambda->getValue() );
    //  const RealPos&         curVal  = static_cast<const RealPos&>( nodePtr->getValue() );
    
    // Generate new allocation vector
    Mixture mix = static_cast< const Mixture&>( mixture->getValue() );
    std::vector<int>  newClass = RbStatistics::Multinomial::rv(mix.getClassProbabilities().getValue(), 1, *rng);
    int oldClassInt = (mix.getElementAllocation(static_cast<const Integer&>(index->getValue() ) ) ).getValue();
    std::vector <int> oldClass;
    oldClass.push_back(oldClassInt);
    // Generate new value (no reflection, so we simply abort later if we propose value here outside of support)
  //  RealPos u      = rng->uniform01();
    //   RealPos newVal = curVal * std::exp( l * ( u - 0.5 ) );
    
    // compute the Hastings ratio
    double pnew = RbStatistics::Multinomial::lnPdf (mix.getClassProbabilities().getValue(), newClass);
    double pold = RbStatistics::Multinomial::lnPdf (mix.getClassProbabilities().getValue(),  oldClass);
    double lnHastingsratio = pnew - pold ; //log( newVal / curVal );
    
    // Propose new value
    //  nodePtr->setValue( newVal.clone() );
    
    return lnHastingsratio;
}



/** We catch here the setting of the member variables to store our parameters. */
void Move_reallocate::setMemberVariable(std::string const &name, Variable* var) {
    
    // test whether we want to set the variable 
    if ( name == "mixture" ) {
        mixture = var;
    } 
    else if ( name == "index" ) {
        index = var;
    }
    else {
        MoveSimple::setMemberVariable(name, var);
    }
}

