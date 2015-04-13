//
//  Move_LevyJumpSum.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 4/4/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "RlBoolean.h"
#include "ContinuousStochasticNode.h"
#include "ModelVector.h"
#include "Move_LevyJumpSum.h"
#include "Natural.h"
#include "LevyJumpSumMove.h"
#include "RbException.h"
#include "Real.h"
#include "RealPos.h"
#include "RevObject.h"
#include "RlTimeTree.h"
#include "TimeTree.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"



using namespace RevLanguage;

Move_LevyJumpSum::Move_LevyJumpSum() : Move() {
    
}

/** Clone object */
Move_LevyJumpSum* Move_LevyJumpSum::clone(void) const {
    
	return new Move_LevyJumpSum(*this);
}


void Move_LevyJumpSum::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    double sf = static_cast<const RealPos &>( slideFactor->getRevObject() ).getValue();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    
    RevBayesCore::TypedDagNode<double>* tmpsv1 = static_cast<const Real &>( slideUp->getRevObject() ).getDagNode();
    RevBayesCore::ContinuousStochasticNode *sv1 = static_cast<RevBayesCore::ContinuousStochasticNode *>( tmpsv1 );
    
    RevBayesCore::TypedDagNode<double>* tmpsv2 = static_cast<const Real &>( slideDown->getRevObject() ).getDagNode();
    RevBayesCore::ContinuousStochasticNode *sv2 = static_cast<RevBayesCore::ContinuousStochasticNode *>( tmpsv2 );
    
    
    bool tv = static_cast<const RlBoolean &>( tune->getRevObject() ).getValue();
    
	std::vector<RevBayesCore::DagNode*> valueVec;
	valueVec.push_back( sv1 );
	valueVec.push_back( sv2 );
    
    
    value = new RevBayesCore::LevyJumpSumMove(valueVec, sf, tv, w);
}


/** Get Rev type of object */
const std::string& Move_LevyJumpSum::getClassType(void) {
    
    static std::string revType = "Move_LevyJumpSum";
    
	return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& Move_LevyJumpSum::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return revTypeSpec;
}



/** Return member rules (no members) */
const MemberRules& Move_LevyJumpSum::getParameterRules(void) const {
    
    static MemberRules mixingStepMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        mixingStepMemberRules.push_back( new ArgumentRule( "value_1", Real::getClassTypeSpec()     , ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        mixingStepMemberRules.push_back( new ArgumentRule( "value_2", Real::getClassTypeSpec()     , ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        mixingStepMemberRules.push_back( new ArgumentRule( "slide"  , RealPos::getClassTypeSpec()  , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Real(1.0) ) );
        mixingStepMemberRules.push_back( new ArgumentRule( "tune"   , RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean( false ) ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getParameterRules();
        mixingStepMemberRules.insert( mixingStepMemberRules.end(), inheritedRules.begin(), inheritedRules.end() );
        
        rulesSet = true;
    }
    
    return mixingStepMemberRules;
}

/** Get type spec */
const TypeSpec& Move_LevyJumpSum::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void Move_LevyJumpSum::printValue(std::ostream &o) const {
    
    o << "Move_LevyJumpSum(";
    if (slideUp != NULL) {
        o << slideUp->getName();
    }
    else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void Move_LevyJumpSum::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "value_1" ) {
        slideUp = var;
    }
    else if ( name == "value_2" ) {
        slideDown = var;
    }
    else if ( name == "slide" ) {
        slideFactor = var;
    }
    else if ( name == "weight" ) {
        weight = var;
    }
    else if ( name == "tune" ) {
        tune = var;
    }
    else {
        Move::setConstParameter(name, var);
    }
}
