//
//  NormalDistribution.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/6/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "RlUniformTopologyDistribution.h"
#include "UniformTopologyDistribution.h"
#include "Natural.h"
#include "Real.h"
#include "RealPos.h"
#include "RlString.h"
#include "RlTopology.h"
#include "StochasticNode.h"
#include "Vector.h"

using namespace RevLanguage;

UniformTopologyDistribution::UniformTopologyDistribution() : TypedDistribution<Topology>() {
    
}


UniformTopologyDistribution::~UniformTopologyDistribution() {
    
}



UniformTopologyDistribution* UniformTopologyDistribution::clone( void ) const {
    return new UniformTopologyDistribution(*this);
}


RevBayesCore::UniformTopologyDistribution* UniformTopologyDistribution::createDistribution( void ) const {
    // get the parameters
    int n = static_cast<const Natural &>( numTaxa->getValue() ).getValueNode()->getValue();
    const std::vector<std::string> &names = static_cast<const Vector<RlString> &>( taxonNames->getValue() ).getValueNode()->getValue();
    RevBayesCore::UniformTopologyDistribution*   d = new RevBayesCore::UniformTopologyDistribution(n, names);
    
    return d;
}



/* Get class name of object */
const std::string& UniformTopologyDistribution::getClassName(void) { 
    
    static std::string rbClassName = "uniform topology distribution";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& UniformTopologyDistribution::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass; 
}




/** Return member rules (no members) */
const MemberRules& UniformTopologyDistribution::getMemberRules(void) const {
    
    static MemberRules distUniformTopologyMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        distUniformTopologyMemberRules.push_back( new ArgumentRule( "nTaxa"  , true, Natural::getClassTypeSpec() ) );
        distUniformTopologyMemberRules.push_back( new ArgumentRule( "names"  , true, Vector<RlString>::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return distUniformTopologyMemberRules;
}


const TypeSpec& UniformTopologyDistribution::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void UniformTopologyDistribution::printValue(std::ostream& o) const {
    
    o << " uniform topology (nTaxa=";
    if ( numTaxa != NULL ) {
        o << numTaxa->getName();
    } else {
        o << "?";
    }
    o << ", names=";
    if ( taxonNames != NULL ) {
        o << taxonNames->getName();
    } else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void UniformTopologyDistribution::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) {
    
    if ( name == "nTaxa" ) {
        numTaxa = var;
    }
    else if ( name == "names" ) {
        taxonNames = var;
    }
    else {
        Distribution::setConstMemberVariable(name, var);
    }
}
