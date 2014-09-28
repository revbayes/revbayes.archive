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
#include "ModelVector.h"
#include "Real.h"
#include "RealPos.h"
#include "RlSimplex.h"
#include "StochasticNode.h"

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
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* a = static_cast<const ModelVector<RealPos> &>( alpha->getRevObject() ).getDagNode();
    RevBayesCore::DirichletDistribution* d              = new RevBayesCore::DirichletDistribution( a );
    
    return d;
}



/* Get Rev type of object */
const std::string& Dist_dirichlet::getClassType(void) { 
    
    static std::string revType = "Dist_dirichlet";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_dirichlet::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( TypedDistribution<Simplex>::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}




/** Return member rules (no members) */
const MemberRules& Dist_dirichlet::getMemberRules(void) const {
    
    static MemberRules distExpMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        distExpMemberRules.push_back( new ArgumentRule( "alpha", ModelVector<RealPos>::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        
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
