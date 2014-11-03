/* 
 * File:   Dist_autocorrelatedLnBranchRates.cpp
 * Author: Tracy Heath
 * 
 */

#include "ArgumentRule.h"
#include "Dist_autocorrelatedLnBranchRates.h"
#include "ModelVector.h"
#include "Real.h"
#include "RealPos.h"
#include "RealNodeContainer.h"
#include "RlTimeTree.h"
#include "TimeTree.h"

using namespace RevLanguage;


Dist_autocorrelatedLnBranchRates* Dist_autocorrelatedLnBranchRates::clone( void ) const {
    return new Dist_autocorrelatedLnBranchRates(*this);
}


RevBayesCore::AutocorrelatedLognormalRateDistribution* Dist_autocorrelatedLnBranchRates::createDistribution( void ) const {
    // get the parameters

    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree>* tau = static_cast<const TimeTree &>( tree->getRevObject() ).getDagNode();
    
    RevBayesCore::TypedDagNode<double>* s  = static_cast<const Real&>( sigma->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* rr  = static_cast<const Real&>( rootRate->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* sv  = static_cast<const Real&>( scaleVal->getRevObject() ).getDagNode();
    
    RevBayesCore::AutocorrelatedLognormalRateDistribution* d = new RevBayesCore::AutocorrelatedLognormalRateDistribution( tau, s, rr, sv );
    
    return d;

}



/* Get Rev type of object */
const std::string& Dist_autocorrelatedLnBranchRates::getClassType(void) {
    
    static std::string revType = "Dist_autocorrelatedLnBranchRates";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_autocorrelatedLnBranchRates::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( TypedDistribution< ModelVector<RealPos> >::getClassTypeSpec() ) );
    
	return revTypeSpec;
}



/** Return member rules (no members) */
const MemberRules& Dist_autocorrelatedLnBranchRates::getParameterRules(void) const {
    
    static MemberRules dist;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        dist.push_back( new ArgumentRule( "tree"      , TimeTree::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        dist.push_back( new ArgumentRule( "sigma"     , RealPos::getClassTypeSpec() , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        dist.push_back( new ArgumentRule( "rootRate"  , Real::getClassTypeSpec()    , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        dist.push_back( new ArgumentRule( "scaleValue", RealPos::getClassTypeSpec() , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        rulesSet = true;
        
    }
    
    return dist;
}


const TypeSpec& Dist_autocorrelatedLnBranchRates::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}




/** Print value for user */

 void Dist_autocorrelatedLnBranchRates::printValue(std::ostream& o) const {
    
    o << " autocorrelatedLnBranchRates(";
    
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
     
     o << "rootRate=";
     if ( rootRate != NULL ) {
         o << rootRate->getName();
     } else {
         o << "?";
     }

     o << ",";
     
     o << "scaleValue=";
     if ( scaleVal != NULL ) {
         o << scaleVal->getName();
     } else {
         o << "?";
     }

     o << ")";
}



/** Set a member variable */
void Dist_autocorrelatedLnBranchRates::setConstParameter(const std::string& name, const RevPtr<const Variable> &var) {

    if ( name == "tree" )
    {
        tree = var;
    }
    else if ( name == "sigma" )
    {
        sigma = var;
    }
    else if ( name == "rootRate" )
    {
        rootRate = var;
    }
    else if ( name == "scaleValue" )
    {
        scaleVal = var;
    }
    else {
        Distribution::setConstParameter(name, var);
    }
}

