//
//  MoveSlide.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/6/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "RlBoolean.h"
#include "ContinuousStochasticNode.h"
#include "Move_VectorSingleElementScale.h"
#include "Natural.h"
#include "RbException.h"
#include "Real.h"
#include "RealPos.h"
#include "RevObject.h"
#include "Vector.h"
#include "VectorSingleElementScaleMove.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

Move_VectorSingleElementScale::Move_VectorSingleElementScale() : Move() {
    
}

/** Clone object */
Move_VectorSingleElementScale* Move_VectorSingleElementScale::clone(void) const {
    
	return new Move_VectorSingleElementScale(*this);
}


void Move_VectorSingleElementScale::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    double l = static_cast<const RealPos &>( lambda->getRevObject() ).getValue();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::TypedDagNode<std::vector<double> >* tmp = static_cast<const Vector<RealPos> &>( v->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode<std::vector<double> > *n = static_cast<RevBayesCore::StochasticNode<std::vector<double> > *>( tmp );
    bool t = static_cast<const RlBoolean &>( tune->getRevObject() ).getValue();
    value = new RevBayesCore::VectorSingleElementScaleMove(n, l, t, w);
}


/** Get class name of object */
const std::string& Move_VectorSingleElementScale::getClassName(void) { 
    
    static std::string rbClassName = "Move_VectorSingleElementScale";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Move_VectorSingleElementScale::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return rbClass; 
}



/** Return member rules (no members) */
const MemberRules& Move_VectorSingleElementScale::getMemberRules(void) const {
    
    static MemberRules scalingMoveMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        scalingMoveMemberRules.push_back( new ArgumentRule( "x", false, Vector<RealPos>::getClassTypeSpec() ) );
        scalingMoveMemberRules.push_back( new ArgumentRule( "lambda", true, RealPos::getClassTypeSpec() , new Real(1.0) ) );
        scalingMoveMemberRules.push_back( new ArgumentRule( "tune"  , true, RlBoolean::getClassTypeSpec(), new RlBoolean( true ) ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getMemberRules();
        scalingMoveMemberRules.insert( scalingMoveMemberRules.end(), inheritedRules.begin(), inheritedRules.end() ); 
        
        rulesSet = true;
    }
    
    return scalingMoveMemberRules;
}

/** Get type spec */
const TypeSpec& Move_VectorSingleElementScale::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void Move_VectorSingleElementScale::printValue(std::ostream &o) const {
    
    o << "Move_VectorSingleElementScale(";
    if (v != NULL) {
        o << v->getName();
    }
    else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void Move_VectorSingleElementScale::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "x" ) {
        v = var;
    }
    else if ( name == "lambda" ) {
        lambda = var;
    }
    else if ( name == "weight" ) {
        weight = var;
    }
    else if ( name == "tune" ) {
        tune = var;
    }
    else {
        Move::setConstMemberVariable(name, var);
    }
}
