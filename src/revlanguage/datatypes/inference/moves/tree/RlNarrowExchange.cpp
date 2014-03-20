//
//  MoveSlide.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 24/2/14.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "RbLanguageObject.h"
#include "RbException.h"
#include "RealPos.h"
#include "RlNarrowExchange.h"
#include "RlTimeTree.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

NarrowExchange::NarrowExchange() : Move() {
    
}


/** Clone object */
NarrowExchange* NarrowExchange::clone(void) const {
    
	return new NarrowExchange(*this);
}


void NarrowExchange::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree> *tmp = static_cast<const TimeTree &>( tree->getValue() ).getValueNode();
    double w = static_cast<const RealPos &>( weight->getValue() ).getValue();
    RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *t = static_cast<RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *>( tmp );
    value = new RevBayesCore::NarrowExchange(t, w);
}


/** Get class name of object */
const std::string& NarrowExchange::getClassName(void) { 
    
    static std::string rbClassName = "NarrowExchange";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& NarrowExchange::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return rbClass; 
}



/** Return member rules (no members) */
const MemberRules& NarrowExchange::getMemberRules(void) const {
    
    static MemberRules narrowMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        narrowMemberRules.push_back( new ArgumentRule( "tree", false, TimeTree::getClassTypeSpec() ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getMemberRules();
        narrowMemberRules.insert( narrowMemberRules.end(), inheritedRules.begin(), inheritedRules.end() ); 
        
        rulesSet = true;
    }
    
    return narrowMemberRules;
}

/** Get type spec */
const TypeSpec& NarrowExchange::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Get type spec */
void NarrowExchange::printValue(std::ostream &o) const {
    
    o << "Narrow(";
    if (tree != NULL) {
        o << tree->getName();
    }
    else {
        o << "?";
    }
    o << ")";
}


/** Set a NearestNeighborInterchange variable */
void NarrowExchange::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) {
    
    if ( name == "tree" ) 
    {
        tree = var;
    }
    else {
        Move::setConstMemberVariable(name, var);
    }
}
