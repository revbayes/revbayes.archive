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
#include "Move_Simplex.h"
#include "RlSimplex.h"
#include "SimplexMove.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

Move_Simplex::Move_Simplex() : Move() {
    
}


/** Clone object */
Move_Simplex* Move_Simplex::clone(void) const {
    
	return new Move_Simplex(*this);
}


void Move_Simplex::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    double a = static_cast<const RealPos &>( alpha->getRevObject() ).getValue();
    int nc = static_cast<const Natural &>( numCats->getRevObject() ).getValue();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    double o = static_cast<const RealPos &>( offset->getRevObject() ).getValue();
    RevBayesCore::TypedDagNode<std::vector<double> >* tmp = static_cast<const Simplex &>( x->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode<std::vector<double> > *n = static_cast<RevBayesCore::StochasticNode<std::vector<double> > *>( tmp );
    bool t = static_cast<const RlBoolean &>( tune->getRevObject() ).getValue();
    value = new RevBayesCore::SimplexMove(n, a, size_t(nc), o, t, w);
}


/** Get Rev type of object */
const std::string& Move_Simplex::getClassType(void) { 
    
    static std::string revType = "Move_Simplex";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Move_Simplex::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}



/** Return member rules (no members) */
const MemberRules& Move_Simplex::getMemberRules(void) const {
    
    static MemberRules scalingMoveMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        scalingMoveMemberRules.push_back( new ArgumentRule( "x", false, Simplex::getClassTypeSpec() ) );
        scalingMoveMemberRules.push_back( new ArgumentRule( "alpha", true, RealPos::getClassTypeSpec() , new Real(1.0) ) );
        scalingMoveMemberRules.push_back( new ArgumentRule( "numCats", true, Natural::getClassTypeSpec() , new Natural(1) ) );
        scalingMoveMemberRules.push_back( new ArgumentRule( "offset", true, Natural::getClassTypeSpec() , new RealPos(0.0) ) );
        scalingMoveMemberRules.push_back( new ArgumentRule( "tune"  , true, RlBoolean::getClassTypeSpec(), new RlBoolean( true ) ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getMemberRules();
        scalingMoveMemberRules.insert( scalingMoveMemberRules.end(), inheritedRules.begin(), inheritedRules.end() ); 
        
        rulesSet = true;
    }
    
    return scalingMoveMemberRules;
}

/** Get type spec */
const TypeSpec& Move_Simplex::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void Move_Simplex::printValue(std::ostream &o) const {
    
    o << "Move_Simplex(";
    if (x != NULL) {
        o << x->getName();
    }
    else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void Move_Simplex::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "x" ) {
        x = var;
    }
    else if ( name == "alpha" ) {
        alpha = var;
    }
    else if ( name == "numCats" ) {
        numCats = var;
    }
    else if ( name == "offset" ) {
        offset = var;
    }
    else if ( name == "tune" ) {
        tune = var;
    }
    else {
        Move::setConstMemberVariable(name, var);
    }
}
