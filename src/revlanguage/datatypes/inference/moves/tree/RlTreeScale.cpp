//
//  MoveSlide.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/6/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "RbLanguageObject.h"
#include "RbException.h"
#include "RealPos.h"
#include "RlBoolean.h"
#include "RlTreeScale.h"
#include "RlTimeTree.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

TreeScale::TreeScale() : Move() {
    
}


/** Clone object */
TreeScale* TreeScale::clone(void) const {
    
	return new TreeScale(*this);
}


void TreeScale::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree> *tmp = static_cast<const TimeTree &>( tree->getValue() ).getValueNode();
    double w = static_cast<const RealPos &>( weight->getValue() ).getValue();
    double l = static_cast<const RealPos &>( lambda->getValue() ).getValue();
    bool tune = static_cast<const RlBoolean &>( tuning->getValue() ).getValue();
    RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *t = static_cast<RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *>( tmp );
    value = new RevBayesCore::TreeScale(t, w, l, tune);
}


/** Get class name of object */
const std::string& TreeScale::getClassName(void) { 
    
    static std::string rbClassName = "TreeScale";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& TreeScale::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return rbClass; 
}



/** Return member rules (no members) */
const MemberRules& TreeScale::getMemberRules(void) const {
    
    static MemberRules nniMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        nniMemberRules.push_back( new ArgumentRule( "tree", false, TimeTree::getClassTypeSpec() ) );
        nniMemberRules.push_back( new ArgumentRule( "lambda", true, RealPos::getClassTypeSpec(), new RealPos( 1.0 ) ) );
        nniMemberRules.push_back( new ArgumentRule( "tuning", true, RlBoolean::getClassTypeSpec(), new RlBoolean( true ) ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getMemberRules();
        nniMemberRules.insert( nniMemberRules.end(), inheritedRules.begin(), inheritedRules.end() ); 
        
        rulesSet = true;
    }
    
    return nniMemberRules;
}

/** Get type spec */
const TypeSpec& TreeScale::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Get type spec */
void TreeScale::printValue(std::ostream &o) const {
    
    o << "TreeScale(";
    if (tree != NULL) {
        o << tree->getName();
    }
    else {
        o << "?";
    }
    o << ")";
}


/** Set a NearestNeighborInterchange variable */
void TreeScale::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) {
    
    if ( name == "tree" ) {
        tree = var;
    }
    else if ( name == "lambda" ) {
        lambda = var;
    }
    else if ( name == "tuning" ) {
        tuning = var;
    }
    else {
        Move::setConstMemberVariable(name, var);
    }
}
