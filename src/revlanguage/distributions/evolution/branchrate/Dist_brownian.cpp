//
//  Dist_brownian.cpp
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-22.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//



#include "ArgumentRule.h"
#include "BrownianPhyloProcess.h"
#include "Dist_brownian.h"
#include "ModelVector.h"
#include "Real.h"
#include "RealNodeContainer.h"
#include "RlTimeTree.h"
#include "StochasticNode.h"
#include "TimeTree.h"

using namespace RevLanguage;


Dist_brownian* Dist_brownian::clone( void ) const {
    return new Dist_brownian(*this);
}


RevBayesCore::BrownianPhyloProcess* Dist_brownian::createDistribution( void ) const {
    // get the parameters

    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree>* tau = static_cast<const TimeTree &>( tree->getRevObject() ).getDagNode();
    
    RevBayesCore::TypedDagNode<double>* s  = static_cast<const RealPos&>( sigma->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* dr  = static_cast<const Real&>( drift->getRevObject() ).getDagNode();
    
    RevBayesCore::BrownianPhyloProcess* d    = new RevBayesCore::BrownianPhyloProcess( tau, s, dr );
    
    return d;

}



/* Get Rev type of object */
const std::string& Dist_brownian::getClassType(void) {
    
    static std::string revType = "Dist_brownian";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_brownian::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return revTypeSpec;
}



/** Return member rules (no members) */
const MemberRules& Dist_brownian::getParameterRules(void) const {
    
    static MemberRules dist;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        dist.push_back( new ArgumentRule( "tree" , TimeTree::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        dist.push_back( new ArgumentRule( "sigma", RealPos::getClassTypeSpec() , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        dist.push_back( new ArgumentRule( "drift", Real::getClassTypeSpec()    , ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new Real(0) ) );
        rulesSet = true;
    }
    
    return dist;
}


const TypeSpec& Dist_brownian::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}




/** Print value for user */

 void Dist_brownian::printValue(std::ostream& o) const {
    
    o << " brownian(";
    
    o << "tau=";
    if ( tree != NULL ) {
        o << tree->getName();
    } else {
        o << "?";
    }

     o << ",";
     
     o << "sigma=";
     if ( sigma != NULL ) {
         o << sigma->getName();
     } else {
         o << "?";
     }

     o << ",";
     
     o << "drift=";
     if ( drift != NULL ) {
         o << drift->getName();
     } else {
         o << "?";
     }

     o << ")";
}



/** Set a member variable */
void Dist_brownian::setConstParameter(const std::string& name, const RevPtr<const Variable> &var) {

    if ( name == "tree" )
    {
        tree = var;
    }
    else if ( name == "sigma" )
    {
        sigma = var;
    }
    else if ( name == "drift" )
    {
        drift = var;
    }
    else {
        Distribution::setConstParameter(name, var);
    }
}

