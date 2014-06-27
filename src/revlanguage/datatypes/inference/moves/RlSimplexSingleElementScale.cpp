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
#include "Natural.h"
#include "RevObject.h"
#include "RbException.h"
#include "Real.h"
#include "RealPos.h"
#include "RlSimplexSingleElementScale.h"
#include "RlSimplex.h"
#include "SimplexSingleElementScale.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

SimplexSingleElementScale::SimplexSingleElementScale() : Move() {
    
}


/** Clone object */
SimplexSingleElementScale* SimplexSingleElementScale::clone(void) const {
    
	return new SimplexSingleElementScale(*this);
}


void SimplexSingleElementScale::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    double a = static_cast<const RealPos &>( alpha->getRevObject() ).getValue();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::TypedDagNode<std::vector<double> >* tmp = static_cast<const Simplex &>( x->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode<std::vector<double> > *n = static_cast<RevBayesCore::StochasticNode<std::vector<double> > *>( tmp );
    bool t = static_cast<const RlBoolean &>( tune->getRevObject() ).getValue();
    value = new RevBayesCore::SimplexSingleElementScale(n, a, t, w);
}


/** Get class name of object */
const std::string& SimplexSingleElementScale::getClassName(void) { 
    
    static std::string rbClassName = "SimplexSingleElementScale";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& SimplexSingleElementScale::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return rbClass; 
}



/** Return member rules (no members) */
const MemberRules& SimplexSingleElementScale::getMemberRules(void) const {
    
    static MemberRules scalingMoveMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        scalingMoveMemberRules.push_back( new ArgumentRule( "x", false, Simplex::getClassTypeSpec() ) );
        scalingMoveMemberRules.push_back( new ArgumentRule( "alpha", true, RealPos::getClassTypeSpec() , new Real(1.0) ) );
        scalingMoveMemberRules.push_back( new ArgumentRule( "tune"  , true, RlBoolean::getClassTypeSpec(), new RlBoolean( true ) ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getMemberRules();
        scalingMoveMemberRules.insert( scalingMoveMemberRules.end(), inheritedRules.begin(), inheritedRules.end() ); 
        
        rulesSet = true;
    }
    
    return scalingMoveMemberRules;
}

/** Get type spec */
const TypeSpec& SimplexSingleElementScale::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void SimplexSingleElementScale::printValue(std::ostream &o) const {
    
    o << "SimplexSingleElementScale(";
    if (x != NULL) {
        o << x->getName();
    }
    else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void SimplexSingleElementScale::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "x" ) {
        x = var;
    }
    else if ( name == "alpha" ) {
        alpha = var;
    }
    else if ( name == "tune" ) {
        tune = var;
    }
    else {
        Move::setConstMemberVariable(name, var);
    }
}
