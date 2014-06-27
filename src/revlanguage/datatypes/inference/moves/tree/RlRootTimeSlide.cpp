//
//  MoveSlide.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/6/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "RevObject.h"
#include "RbException.h"
#include "RealPos.h"
#include "RlBoolean.h"
#include "RlRootTimeSlide.h"
#include "RlTimeTree.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

RootTimeSlide::RootTimeSlide() : Move() {
    
}


/** Clone object */
RootTimeSlide* RootTimeSlide::clone(void) const {
    
	return new RootTimeSlide(*this);
}


void RootTimeSlide::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree> *tmp = static_cast<const TimeTree &>( tree->getValue() ).getValueNode();
    double d = static_cast<const RealPos &>( delta->getValue() ).getValue();
    bool at = static_cast<const RlBoolean &>( tune->getValue() ).getValue();
    double w = static_cast<const RealPos &>( weight->getValue() ).getValue();
    RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *t = static_cast<RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *>( tmp );
    value = new RevBayesCore::RootTimeSlide(t, d, at, w);
}


/** Get class name of object */
const std::string& RootTimeSlide::getClassName(void) { 
    
    static std::string rbClassName = "RootTimeSlide";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& RootTimeSlide::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return rbClass; 
}



/** Return member rules (no members) */
const MemberRules& RootTimeSlide::getMemberRules(void) const {
    
    static MemberRules nniMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
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
const TypeSpec& RootTimeSlide::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Get type spec */
void RootTimeSlide::printValue(std::ostream &o) const {
    
    o << "RootTimeSlide(";
    if (tree != NULL) {
        o << tree->getName();
    }
    else {
        o << "?";
    }
    o << ")";
}


/** Set a NearestNeighborInterchange variable */
void RootTimeSlide::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "tree" ) {
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
