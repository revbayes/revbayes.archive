//
//  MoveSlide.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/6/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Natural.h"
#include "RevObject.h"
#include "RbException.h"
#include "RealPos.h"
#include "RlWeightedNodeTimeSlide.h"
#include "RlTimeTree.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

WeightedNodeTimeSlide::WeightedNodeTimeSlide() : Move() {
    
}


/** Clone object */
WeightedNodeTimeSlide* WeightedNodeTimeSlide::clone(void) const {
    
	return new WeightedNodeTimeSlide(*this);
}


void WeightedNodeTimeSlide::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree> *tmp = static_cast<const TimeTree &>( tree->getValue() ).getValueNode();
    int b = static_cast<const Natural &>( blocks->getValue() ).getValue();
    double w = static_cast<const RealPos &>( weight->getValue() ).getValue();
    RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *t = static_cast<RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *>( tmp );
    value = new RevBayesCore::WeightedNodeTimeSlide(t, size_t(b), w);
}


/** Get class name of object */
const std::string& WeightedNodeTimeSlide::getClassName(void) { 
    
    static std::string rbClassName = "WeightedNodeTimeSlide";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& WeightedNodeTimeSlide::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return rbClass; 
}



/** Return member rules (no members) */
const MemberRules& WeightedNodeTimeSlide::getMemberRules(void) const {
    
    static MemberRules nniMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        nniMemberRules.push_back( new ArgumentRule( "tree", false, TimeTree::getClassTypeSpec() ) );
        nniMemberRules.push_back( new ArgumentRule( "blocks", true, Natural::getClassTypeSpec(), new Natural(8) ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getMemberRules();
        nniMemberRules.insert( nniMemberRules.end(), inheritedRules.begin(), inheritedRules.end() ); 
        
        rulesSet = true;
    }
    
    return nniMemberRules;
}

/** Get type spec */
const TypeSpec& WeightedNodeTimeSlide::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Get type spec */
void WeightedNodeTimeSlide::printValue(std::ostream &o) const {
    
    o << "WeightedNodeTimeSlide(";
    if (tree != NULL) {
        o << tree->getName();
    }
    else {
        o << "?";
    }
    o << ")";
}


/** Set a NearestNeighborInterchange variable */
void WeightedNodeTimeSlide::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) {
    
    if ( name == "tree" ) {
        tree = var;
    }
    else if ( name == "blocks" ) {
        blocks = var;
    }
    else {
        Move::setConstMemberVariable(name, var);
    }
}
