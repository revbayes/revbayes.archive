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
#include "RlNodeTimeSlideUniform.h"
#include "RlTimeTree.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

NodeTimeSlideUniform::NodeTimeSlideUniform() : Move() {
    
}


/** Clone object */
NodeTimeSlideUniform* NodeTimeSlideUniform::clone(void) const {
    
	return new NodeTimeSlideUniform(*this);
}


void NodeTimeSlideUniform::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree> *tmp = static_cast<const TimeTree &>( tree->getValue() ).getValueNode();
    double w = static_cast<const RealPos &>( weight->getValue() ).getValue();
    RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *t = static_cast<RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *>( tmp );
    value = new RevBayesCore::NodeTimeSlideUniform(t, w);
}


/** Get class name of object */
const std::string& NodeTimeSlideUniform::getClassName(void) { 
    
    static std::string rbClassName = "NodeTimeSlideUniform";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& NodeTimeSlideUniform::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return rbClass; 
}



/** Return member rules (no members) */
const MemberRules& NodeTimeSlideUniform::getMemberRules(void) const {
    
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
const TypeSpec& NodeTimeSlideUniform::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Get type spec */
void NodeTimeSlideUniform::printValue(std::ostream &o) const {
    
    o << "NodeTimeSlideUniform(";
    if (tree != NULL) {
        o << tree->getName();
    }
    else {
        o << "?";
    }
    o << ")";
}


/** Set a NearestNeighborInterchange variable */
void NodeTimeSlideUniform::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) {
    
    if ( name == "tree" ) {
        tree = var;
    }
    else {
        Move::setConstMemberVariable(name, var);
    }
}
