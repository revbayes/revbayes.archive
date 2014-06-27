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
#include "RlFixedNodeheightPruneRegraft.h"
#include "RlTimeTree.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

FixedNodeheightPruneRegraft::FixedNodeheightPruneRegraft() : Move() {
    
}


/** Clone object */
FixedNodeheightPruneRegraft* FixedNodeheightPruneRegraft::clone(void) const {
    
	return new FixedNodeheightPruneRegraft(*this);
}


void FixedNodeheightPruneRegraft::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree> *tmp = static_cast<const TimeTree &>( tree->getRevObject() ).getDagNode();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *t = static_cast<RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *>( tmp );
    value = new RevBayesCore::FixedNodeheightPruneRegraft(t, w);
}


/** Get class name of object */
const std::string& FixedNodeheightPruneRegraft::getClassName(void) { 
    
    static std::string rbClassName = "Move_FNPR";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& FixedNodeheightPruneRegraft::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return rbClass; 
}



/** Return member rules (no members) */
const MemberRules& FixedNodeheightPruneRegraft::getMemberRules(void) const {
    
    static MemberRules nniMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        nniMemberRules.push_back( new ArgumentRule( "tree", false, TimeTree::getClassTypeSpec() ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getMemberRules();
        nniMemberRules.insert( nniMemberRules.end(), inheritedRules.begin(), inheritedRules.end() ); 
        
        rulesSet = true;
    }
    
    return nniMemberRules;
}

/** Get type spec */
const TypeSpec& FixedNodeheightPruneRegraft::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Get type spec */
void FixedNodeheightPruneRegraft::printValue(std::ostream &o) const {
    
    o << "FNPR(";
    if (tree != NULL) {
        o << tree->getName();
    }
    else {
        o << "?";
    }
    o << ")";
}


/** Set a NearestNeighborInterchange variable */
void FixedNodeheightPruneRegraft::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "tree" ) {
        tree = var;
    }
    else {
        Move::setConstMemberVariable(name, var);
    }
}
