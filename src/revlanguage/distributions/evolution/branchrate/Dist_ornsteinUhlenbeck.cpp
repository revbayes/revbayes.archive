/* 
 * File:   Dist_ornsteinUhlenbeck.cpp
 * Author: nl
 * 
 * Created on 26 juillet 2014, 15:24
 */

#include "ArgumentRule.h"
#include "Dist_ornsteinUhlenbeck.h"
#include "ModelVector.h"
#include "Real.h"
#include "RealPos.h"
#include "RealNodeContainer.h"
#include "RlTimeTree.h"
#include "StochasticNode.h"
#include "TimeTree.h"

using namespace RevLanguage;


Dist_ornsteinUhlenbeck* Dist_ornsteinUhlenbeck::clone( void ) const {
    return new Dist_ornsteinUhlenbeck(*this);
}


RevBayesCore::OrnsteinUhlenbeckPhyloProcess* Dist_ornsteinUhlenbeck::createDistribution( void ) const {
    // get the parameters

    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree>* tau = static_cast<const TimeTree &>( tree->getRevObject() ).getDagNode();
    
    RevBayesCore::TypedDagNode<double>* s  = static_cast<const Real&>( sigma->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* m  = static_cast<const Real&>( mean->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* p  = static_cast<const Real&>( phi->getRevObject() ).getDagNode();
    
    RevBayesCore::OrnsteinUhlenbeckPhyloProcess* d    = new RevBayesCore::OrnsteinUhlenbeckPhyloProcess( tau, s, m, p );
    
    return d;

}



/* Get Rev type of object */
const std::string& Dist_ornsteinUhlenbeck::getClassType(void) {
    
    static std::string revType = "Dist_ornsteinUhlenbeck";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_ornsteinUhlenbeck::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return revTypeSpec;
}



/** Return member rules (no members) */
const MemberRules& Dist_ornsteinUhlenbeck::getParameterRules(void) const {
    
    static MemberRules dist;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        dist.push_back( new ArgumentRule( "tree" , TimeTree::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        dist.push_back( new ArgumentRule( "sigma", RealPos::getClassTypeSpec() , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        dist.push_back( new ArgumentRule( "mean" , Real::getClassTypeSpec()    , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        dist.push_back( new ArgumentRule( "phi"  , RealPos::getClassTypeSpec() , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        rulesSet = true;
    }
    
    return dist;
}


const TypeSpec& Dist_ornsteinUhlenbeck::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}




/** Print value for user */

 void Dist_ornsteinUhlenbeck::printValue(std::ostream& o) const {
    
    o << " ornsteinUhlenbeck(";
    
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
     
     o << "mean=";
     if ( mean != NULL ) {
         o << mean->getName();
     } else {
         o << "?";
     }

     o << ",";
     
     o << "phi=";
     if ( phi != NULL ) {
         o << phi->getName();
     } else {
         o << "?";
     }

     o << ")";
}



/** Set a member variable */
void Dist_ornsteinUhlenbeck::setConstParameter(const std::string& name, const RevPtr<const Variable> &var) {

    if ( name == "tree" )
    {
        tree = var;
    }
    else if ( name == "sigma" )
    {
        sigma = var;
    }
    else if ( name == "mean" )
    {
        mean = var;
    }
    else if ( name == "phi" )
    {
        phi = var;
    }
    else {
        Distribution::setConstParameter(name, var);
    }
}

