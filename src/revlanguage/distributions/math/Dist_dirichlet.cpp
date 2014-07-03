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
#include "Dist_dirichlet.h"
#include "Real.h"
#include "RealPos.h"
#include "RlSimplex.h"
#include "StochasticNode.h"
#include "Vector.h"

using namespace RevLanguage;

Dist_dirichlet::Dist_dirichlet() : TypedDistribution<Simplex>() {
    
}


Dist_dirichlet::~Dist_dirichlet() {
    
}



Dist_dirichlet* Dist_dirichlet::clone( void ) const {
    return new Dist_dirichlet(*this);
}


RevBayesCore::DirichletDistribution* Dist_dirichlet::createDistribution( void ) const {

    // get the parameters
    RevBayesCore::TypedDagNode<std::vector<double> >* a = static_cast<const Vector<RealPos> &>( alpha->getRevObject() ).getDagNode();
    RevBayesCore::DirichletDistribution* d              = new RevBayesCore::DirichletDistribution( a );
    
    return d;
}



/* Get class name of object */
const std::string& Dist_dirichlet::getClassName(void) { 
    
    static std::string rbClassName = "Dist_dirichlet";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_dirichlet::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass; 
}




/** Return member rules (no members) */
const MemberRules& Dist_dirichlet::getMemberRules(void) const {
    
    static MemberRules distExpMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        distExpMemberRules.push_back( new ArgumentRule( "alpha", true, Vector<RealPos>::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return distExpMemberRules;
}


const TypeSpec& Dist_dirichlet::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void Dist_dirichlet::printValue(std::ostream& o) const {
    
    o << " dirichlet(alpha=";
    if ( alpha != NULL ) {
        o << alpha->getName();
    } else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void Dist_dirichlet::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "alpha" ) 
    {
        alpha = var;
    }
    else 
    {
        Distribution::setConstMemberVariable(name, var);
    }
}
