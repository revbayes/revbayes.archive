//
//  RlWhiteNoiseProcess.cpp
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-22.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//



#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "BrownianPhyloProcess.h"
#include "RlBrownianPhyloProcess.h"
#include "Real.h"
#include "RlTimeTree.h"
#include "TimeTree.h"
#include "Real.h"
#include "StochasticNode.h"
#include "Vector.h"

using namespace RevLanguage;


BrownianPhyloProcess* BrownianPhyloProcess::clone( void ) const {
    return new BrownianPhyloProcess(*this);
}


RevBayesCore::BrownianPhyloProcess* BrownianPhyloProcess::createDistribution( void ) const {
    // get the parameters

    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree>* tau = static_cast<const TimeTree &>( tree->getValue() ).getValueNode();
    
    RevBayesCore::TypedDagNode<double>* s  = static_cast<const Real&>( sigma->getValue() ).getValueNode();
    RevBayesCore::TypedDagNode<double>* r  = static_cast<const Real&>( rootval->getValue() ).getValueNode();
    
    RevBayesCore::BrownianPhyloProcess* d    = new RevBayesCore::BrownianPhyloProcess( tau, s, r );
    
    return d;

}



/* Get class name of object */
const std::string& BrownianPhyloProcess::getClassName(void) {
    
    static std::string rbClassName = "Brownian Process along Phylogeny";
    
	return rbClassName;
}

/* Get class type spec describing type of object */
const TypeSpec& BrownianPhyloProcess::getClassTypeSpec(void) {
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass;
}



/** Return member rules (no members) */
const MemberRules& BrownianPhyloProcess::getMemberRules(void) const {
    
    static MemberRules dist;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        dist.push_back( new ArgumentRule( "tree" , true, TimeTree::getClassTypeSpec() ) );
        dist.push_back( new ArgumentRule( "sigma", true, RealPos::getClassTypeSpec() ) );
        dist.push_back( new ArgumentRule( "rootval", true, Real::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return dist;
}


const TypeSpec& BrownianPhyloProcess::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}




/** Print value for user */

 void BrownianPhyloProcess::printValue(std::ostream& o) const {
    
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
     
     o << "rootval=";
     if ( sigma != NULL ) {
         o << rootval->getName();
     } else {
         o << "?";
     }

     o << ")";
}



/** Set a member variable */
void BrownianPhyloProcess::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) {

    if ( name == "tree" )
    {
        tree = var;
    }
    else if ( name == "sigma" )
    {
        sigma = var;
    }
    else if ( name == "rootval" )
    {
        rootval = var;
    }
    else {
        Distribution::setConstMemberVariable(name, var);
    }
}

