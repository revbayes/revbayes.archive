//
//  MoveSlide.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/6/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Move_OriginTimeSlide.h"
#include "RbException.h"
#include "RealPos.h"
#include "RevObject.h"
#include "RlBoolean.h"
#include "RlTimeTree.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"
#include "OriginTimeSlide.h"

using namespace RevLanguage;

Move_OriginTimeSlide::Move_OriginTimeSlide() : Move() {
    
}


/** Clone object */
Move_OriginTimeSlide* Move_OriginTimeSlide::clone(void) const {
    
	return new Move_OriginTimeSlide(*this);
}


void Move_OriginTimeSlide::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree> *tmp = static_cast<const TimeTree &>( tree->getRevObject() ).getDagNode();
    double d = static_cast<const RealPos &>( delta->getRevObject() ).getValue();
    bool at = static_cast<const RlBoolean &>( tune->getRevObject() ).getValue();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::TypedDagNode<double>* tmpOT = static_cast<const Real &>( originTime->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode< double > *ot = static_cast<RevBayesCore::StochasticNode<double> *>( tmpOT );
    RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *t = static_cast<RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *>( tmp );
    value = new RevBayesCore::OriginTimeSlide(ot, t, d, at, w);
}


/** Get class name of object */
const std::string& Move_OriginTimeSlide::getClassName(void) { 
    
    static std::string rbClassName = "Move_OriginTimeSlide";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Move_OriginTimeSlide::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return rbClass; 
}



/** Return member rules (no members) */
const MemberRules& Move_OriginTimeSlide::getMemberRules(void) const {
    
    static MemberRules nniMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        nniMemberRules.push_back( new ArgumentRule( "origin", false, Real::getClassTypeSpec() ) );
        nniMemberRules.push_back( new ArgumentRule( "tree", false, TimeTree::getClassTypeSpec() ) );
        nniMemberRules.push_back( new ArgumentRule( "delta", true, RealPos::getClassTypeSpec() , new Real(1.0) ) );
        nniMemberRules.push_back( new ArgumentRule( "tune"  , true, RlBoolean::getClassTypeSpec(), new RlBoolean( true ) ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getMemberRules();
        nniMemberRules.insert( nniMemberRules.end(), inheritedRules.begin(), inheritedRules.end() ); 
        
        rulesSet = true;
    }
    
    return nniMemberRules;
}

/** Get type spec */
const TypeSpec& Move_OriginTimeSlide::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Get type spec */
void Move_OriginTimeSlide::printValue(std::ostream &o) const {
    
    o << "Move_OriginTimeSlide(";
    if (originTime != NULL) {
        o << originTime->getName();
    }
    else {
        o << "?";
    }
    o << ")";
}


/** Set a NearestNeighborInterchange variable */
void Move_OriginTimeSlide::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "origin" ) {
        originTime = var;
    }
    else if ( name == "tree" ) {
        tree = var;
    }
    else if ( name == "delta" ) {
        delta = var;
    }
    else if ( name == "tune" ) {
        tune = var;
    }
    else {
        Move::setConstMemberVariable(name, var);
    }
}
