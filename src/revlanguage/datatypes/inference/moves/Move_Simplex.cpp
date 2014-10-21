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
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* tmp = static_cast<const Simplex &>( x->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode< RevBayesCore::RbVector<double> > *n = static_cast<RevBayesCore::StochasticNode< RevBayesCore::RbVector<double> > *>( tmp );
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
const MemberRules& Move_Simplex::getParameterRules(void) const {
    
    static MemberRules moveMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        moveMemberRules.push_back( new ArgumentRule( "x"      , Simplex::getClassTypeSpec()  , ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        moveMemberRules.push_back( new ArgumentRule( "alpha"  , RealPos::getClassTypeSpec()  , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Real(1.0) ) );
        moveMemberRules.push_back( new ArgumentRule( "numCats", Natural::getClassTypeSpec()  , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(1) ) );
        moveMemberRules.push_back( new ArgumentRule( "offset" , Natural::getClassTypeSpec()  , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RealPos(0.0) ) );
        moveMemberRules.push_back( new ArgumentRule( "tune"   , RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean( true ) ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getParameterRules();
        moveMemberRules.insert( moveMemberRules.end(), inheritedRules.begin(), inheritedRules.end() );
        
        rulesSet = true;
    }
    
    return moveMemberRules;
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
void Move_Simplex::setConstParameter(const std::string& name, const RevPtr<const Variable> &var) {
    
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
        Move::setConstParameter(name, var);
    }
}
