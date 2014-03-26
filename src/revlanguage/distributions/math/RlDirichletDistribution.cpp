//
//  ExponentialDistribution.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/6/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "DirichletDistribution.h"
#include "RlDirichletDistribution.h"
#include "Real.h"
#include "RealPos.h"
#include "Simplex.h"
#include "StochasticNode.h"
#include "Vector.h"

using namespace RevLanguage;

DirichletDistribution::DirichletDistribution() : TypedDistribution<Simplex>() {
    
}


DirichletDistribution::~DirichletDistribution() {
    
}



DirichletDistribution* DirichletDistribution::clone( void ) const {
    return new DirichletDistribution(*this);
}


RevBayesCore::DirichletDistribution* DirichletDistribution::createDistribution( void ) const {

    // get the parameters
    RevBayesCore::TypedDagNode<std::vector<double> >* a = static_cast<const Vector<RealPos> &>( alpha->getValue() ).getValueNode();
    RevBayesCore::DirichletDistribution* d    = new RevBayesCore::DirichletDistribution( a );
    
    return d;
}



/* Get class name of object */
const std::string& DirichletDistribution::getClassName(void) { 
    
    static std::string rbClassName = "Dirichlet distribution";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& DirichletDistribution::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass; 
}




/** Return member rules (no members) */
const MemberRules& DirichletDistribution::getMemberRules(void) const {
    
    static MemberRules distExpMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        distExpMemberRules.push_back( new ArgumentRule( "alpha", true, Vector<RealPos>::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return distExpMemberRules;
}


const TypeSpec& DirichletDistribution::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void DirichletDistribution::printValue(std::ostream& o) const {
    
    o << " dirichlet(alpha=";
    if ( alpha != NULL ) {
        o << alpha->getName();
    } else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void DirichletDistribution::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) {
    
    if ( name == "alpha" ) {
        alpha = var;
    }
    else {
        Distribution::setConstMemberVariable(name, var);
    }
}
