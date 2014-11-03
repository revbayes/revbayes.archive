//
//  MoveSlide.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 24/2/14.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Move_NarrowExchange.h"
#include "RbException.h"
#include "RealPos.h"
#include "RevObject.h"
#include "RlTimeTree.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

Move_NarrowExchange::Move_NarrowExchange() : Move() {
    
}


/** Clone object */
Move_NarrowExchange* Move_NarrowExchange::clone(void) const {
    
	return new Move_NarrowExchange(*this);
}


void Move_NarrowExchange::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree> *tmp = static_cast<const TimeTree &>( tree->getRevObject() ).getDagNode();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *t = static_cast<RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *>( tmp );
    value = new RevBayesCore::NarrowExchange(t, w);
}


/** Get Rev type of object */
const std::string& Move_NarrowExchange::getClassType(void) { 
    
    static std::string revType = "Move_NarrowExchange";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Move_NarrowExchange::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}



/** Return member rules (no members) */
const MemberRules& Move_NarrowExchange::getParameterRules(void) const {
    
    static MemberRules narrowMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        narrowMemberRules.push_back( new ArgumentRule( "tree", TimeTree::getClassTypeSpec(), ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getParameterRules();
        narrowMemberRules.insert( narrowMemberRules.end(), inheritedRules.begin(), inheritedRules.end() ); 
        
        rulesSet = true;
    }
    
    return narrowMemberRules;
}

/** Get type spec */
const TypeSpec& Move_NarrowExchange::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Get type spec */
void Move_NarrowExchange::printValue(std::ostream &o) const {
    
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
void Move_NarrowExchange::setConstParameter(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "tree" ) 
    {
        tree = var;
    }
    else {
        Move::setConstParameter(name, var);
    }
}
