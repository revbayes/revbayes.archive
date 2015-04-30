//
//  Move_VectorSlide.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 4/4/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ModelVector.h"
#include "Move_VectorSlide.h"
#include "Natural.h"
#include "RbException.h"
#include "Real.h"
#include "RealPos.h"
#include "RevObject.h"
#include "RlBoolean.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"
#include "VectorSlideMove.h"


using namespace RevLanguage;

Move_VectorSlide::Move_VectorSlide() : Move() {
    
}


/** Clone object */
Move_VectorSlide* Move_VectorSlide::clone(void) const {
    
	return new Move_VectorSlide(*this);
}


void Move_VectorSlide::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new vector-scale move
    double d = static_cast<const RealPos &>( delta->getRevObject() ).getValue();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* tmp = static_cast<const ModelVector<Real> &>( x->getRevObject() ).getDagNode();
    std::set<const RevBayesCore::DagNode*> p = tmp->getParents();
    std::vector< RevBayesCore::StochasticNode<double> *> n;
    for (std::set<const RevBayesCore::DagNode*>::const_iterator it = p.begin(); it != p.end(); ++it)
    {
        const RevBayesCore::StochasticNode<double> *theNode = static_cast< const RevBayesCore::StochasticNode<double>* >( *it );
        n.push_back( const_cast< RevBayesCore::StochasticNode<double>* >( theNode ) );
    }
    bool t = static_cast<const RlBoolean &>( tune->getRevObject() ).getValue();
    value = new RevBayesCore::VectorSlideMove(n, d, t, w);
}


/** Get Rev type of object */
const std::string& Move_VectorSlide::getClassType(void) {
    
    static std::string revType = "Move_VectorSlide";
    
	return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& Move_VectorSlide::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return revTypeSpec;
}



/** Return member rules (no members) */
const MemberRules& Move_VectorSlide::getParameterRules(void) const {
    
    static MemberRules moveMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        moveMemberRules.push_back( new ArgumentRule( "x"     , ModelVector<Real>::getClassTypeSpec(), ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        moveMemberRules.push_back( new ArgumentRule( "delta", RealPos::getClassTypeSpec()             , ArgumentRule::BY_VALUE    , ArgumentRule::ANY, new RealPos(1.0) ) );
        moveMemberRules.push_back( new ArgumentRule( "tune"  , RlBoolean::getClassTypeSpec()           , ArgumentRule::BY_VALUE    , ArgumentRule::ANY, new RlBoolean( true ) ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getParameterRules();
        moveMemberRules.insert( moveMemberRules.end(), inheritedRules.begin(), inheritedRules.end() );
        
        rulesSet = true;
    }
    
    return moveMemberRules;
}

/** Get type spec */
const TypeSpec& Move_VectorSlide::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void Move_VectorSlide::printValue(std::ostream &o) const {
    
    o << "Move_VectorSlide(";
    if (x != NULL) {
        o << x->getName();
    }
    else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void Move_VectorSlide::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "x" ) {
        x = var;
    }
    else if ( name == "delta" ) {
        delta = var;
    }
    else if ( name == "tune" ) {
        tune = var;
    }
    else {
        Move::setConstParameter(name, var);
    }
}
