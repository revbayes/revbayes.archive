//
//  MoveSlide.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/6/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "NearestNeighborInterchange_nonClock.h"
#include "RbLanguageObject.h"
#include "RbException.h"
#include "RealPos.h"
#include "RlNearestNeighborInterchange_nonClock.h"
#include "RlTopology.h"
#include "Topology.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

NearestNeighborInterchange_nonClock::NearestNeighborInterchange_nonClock() : Move() {
    
}


/** Clone object */
NearestNeighborInterchange_nonClock* NearestNeighborInterchange_nonClock::clone(void) const {
    
	return new NearestNeighborInterchange_nonClock(*this);
}


void NearestNeighborInterchange_nonClock::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    RevBayesCore::TypedDagNode<RevBayesCore::Topology> *tmp = static_cast<const Topology &>( tree->getValue() ).getValueNode();
    double w = static_cast<const RealPos &>( weight->getValue() ).getValue();
    RevBayesCore::StochasticNode<RevBayesCore::Topology> *t = static_cast<RevBayesCore::StochasticNode<RevBayesCore::Topology> *>( tmp );
    value = new RevBayesCore::NearestNeighborInterchange_nonClock(t, w);
}


/** Get class name of object */
const std::string& NearestNeighborInterchange_nonClock::getClassName(void) { 
    
    static std::string rbClassName = "NNI";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& NearestNeighborInterchange_nonClock::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return rbClass; 
}



/** Return member rules (no members) */
const MemberRules& NearestNeighborInterchange_nonClock::getMemberRules(void) const {
    
    static MemberRules nniMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        nniMemberRules.push_back( new ArgumentRule( "tree", false, Topology::getClassTypeSpec() ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getMemberRules();
        nniMemberRules.insert( nniMemberRules.end(), inheritedRules.begin(), inheritedRules.end() ); 
        
        rulesSet = true;
    }
    
    return nniMemberRules;
}

/** Get type spec */
const TypeSpec& NearestNeighborInterchange_nonClock::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Get type spec */
void NearestNeighborInterchange_nonClock::printValue(std::ostream &o) const {
    
    o << "NNI(";
    if (tree != NULL) {
        o << tree->getName();
    }
    else {
        o << "?";
    }
    o << ")";
}


/** Set a NearestNeighborInterchange variable */
void NearestNeighborInterchange_nonClock::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) {
    
    if ( name == "tree" ) {
        tree = var;
    }
    else {
        Move::setConstMemberVariable(name, var);
    }
}
