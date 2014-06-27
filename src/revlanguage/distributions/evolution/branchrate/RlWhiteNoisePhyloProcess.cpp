//
//  RlWhiteNoiseProcess.cpp
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-22.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//



#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "WhiteNoisePhyloProcess.h"
#include "RlWhiteNoisePhyloProcess.h"
#include "Real.h"
#include "RlTimeTree.h"
#include "TimeTree.h"
#include "RealPos.h"
#include "StochasticNode.h"
#include "Vector.h"

using namespace RevLanguage;

WhiteNoisePhyloProcess::WhiteNoisePhyloProcess() : TypedDistribution< Vector<RealPos> >() {
    
}


WhiteNoisePhyloProcess::~WhiteNoisePhyloProcess() {
    
}



WhiteNoisePhyloProcess* WhiteNoisePhyloProcess::clone( void ) const {
    return new WhiteNoisePhyloProcess(*this);
}


RevBayesCore::WhiteNoisePhyloProcess* WhiteNoisePhyloProcess::createDistribution( void ) const {
    // get the parameters

    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree>* tau = static_cast<const TimeTree &>( tree->getRevObject() ).getDagNode();
    
    RevBayesCore::TypedDagNode<double>* s  = static_cast<const RealPos &>( sigma->getRevObject() ).getDagNode();
    
    RevBayesCore::WhiteNoisePhyloProcess* d    = new RevBayesCore::WhiteNoisePhyloProcess( tau, s );
    
    return d;

}



/* Get class name of object */
const std::string& WhiteNoisePhyloProcess::getClassName(void) {
    
    static std::string rbClassName = "Dist_whiteNoise";
    
	return rbClassName;
}

/* Get class type spec describing type of object */
const TypeSpec& WhiteNoisePhyloProcess::getClassTypeSpec(void) {
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass;
}



/** Return member rules (no members) */
const MemberRules& WhiteNoisePhyloProcess::getMemberRules(void) const {
    
    static MemberRules dist;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        dist.push_back( new ArgumentRule( "tree" , true, TimeTree::getClassTypeSpec() ) );
        dist.push_back( new ArgumentRule( "sigma", true, RealPos::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return dist;
}


const TypeSpec& WhiteNoisePhyloProcess::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}




/** Print value for user */

 void WhiteNoisePhyloProcess::printValue(std::ostream& o) const {
    
    o << " whitenoise(";
    
    o << "tau=";
    if ( tree != NULL ) {
        o << tree->getName();
    } else {
        o << "?";
    }
    
    o << "sigma=";
    if ( sigma != NULL ) {
        o << sigma->getName();
    } else {
        o << "?";
    }
    o << ")";
}



/** Set a member variable */
void WhiteNoisePhyloProcess::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {

    if ( name == "tree" )
    {
        tree = var;
    }
    else if ( name == "sigma" )
    {
        sigma = var;
    }
    else {
        Distribution::setConstMemberVariable(name, var);
    }
}

