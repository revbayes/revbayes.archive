/* 
 * File:   Dist_InverseWishart.cpp
 * Author: nl
 * 
 * Created on 15 juillet 2014, 10:31
 */

#include "ArgumentRule.h"
#include "Dist_inverseWishart.h"
#include "Natural.h"
#include "RealPos.h"
#include "RealSymmetricMatrix.h"
#include "StochasticNode.h"
#include "InverseWishartDistribution.h"

using namespace RevLanguage;

Dist_inverseWishart::Dist_inverseWishart() : TypedDistribution<RealSymmetricMatrix>() {
    
}


Dist_inverseWishart::~Dist_inverseWishart() {
    
}



Dist_inverseWishart* Dist_inverseWishart::clone( void ) const {
    return new Dist_inverseWishart(*this);
}


RevBayesCore::InverseWishartDistribution* Dist_inverseWishart::createDistribution( void ) const {
    
    // get the parameters
    RevBayesCore::TypedDagNode<RevBayesCore::PrecisionMatrix>* sg = NULL;
    RevBayesCore::TypedDagNode<double>* ka = NULL;
    /*
    if (sigma != NULL)  {
        sg = static_cast<const RealSymmetricMatrix &>( sigma->getRevObject() ).getDagNode();
    }
     */
    if (kappa != NULL)  {
        ka = static_cast<const RealPos&>( kappa->getRevObject() ).getDagNode();
    }
    
    RevBayesCore::TypedDagNode<int>* deg = static_cast<const Natural &>( df->getRevObject()).getDagNode();

    RevBayesCore::TypedDagNode<int>* dm = NULL;
//    int dm = -1;
    if (dim != NULL)    {
        dm = static_cast<const Natural &>( dim->getRevObject()).getDagNode();
//        dm = static_cast<const Natural &>( dim->getValue()).getValue();
    }
    RevBayesCore::InverseWishartDistribution* w    =  0;
    
    if (sg) {
            w = new RevBayesCore::InverseWishartDistribution( sg, deg );
    }
    else{
        if (! dm || ! ka)   {
            throw RbException("error in Dist_inverseWishart: should specify arguments");
        }
        w = new RevBayesCore::InverseWishartDistribution( dm, ka, deg );
    }
    return w;
}



/* Get class name of object */
const std::string& Dist_inverseWishart::getClassName(void) {
    
    static std::string rbClassName = "Dist_inverseWishart";
    
	return rbClassName;
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_inverseWishart::getClassTypeSpec(void) {
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass;
}




/** Return member rules (no members) */
const MemberRules& Dist_inverseWishart::getMemberRules(void) const {
    
    static MemberRules distExpMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        
//        distExpMemberRules.push_back( new ArgumentRule( "sigma", true, RealSymmetricMatrix::getClassTypeSpec() ) );
        distExpMemberRules.push_back( new ArgumentRule( "df", true, Natural::getClassTypeSpec() ) );
        distExpMemberRules.push_back( new ArgumentRule( "kappa", true, RealPos::getClassTypeSpec() ) );
        distExpMemberRules.push_back( new ArgumentRule( "dim", true, Natural::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return distExpMemberRules;
}


const TypeSpec& Dist_inverseWishart::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void Dist_inverseWishart::printValue(std::ostream& o) const {
    
    o << " InverseWishart(sigma=";
/*
    if ( sigma != NULL ) {
        o << sigma->getName();
    } else {
*/
 if (kappa != NULL)  {
            if (dim == NULL)    {
                throw RbException("error in Wishart distribution: kappa and dim should both be non null");
            }
            o << kappa->getName() << ".I_" << dim->getName();
        }
        else{
            o << "?";
        }
   // }
    o << ")";
}


/** Set a member variable */
void Dist_inverseWishart::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "sigma" ) {
//        sigma = var;
    }
    else if ( name == "kappa" ) {
        kappa = var;
    }
    else if ( name == "df" ) {
        df = var;
    }
    else if ( name == "dim" ) {
        dim = var;
    }
    else {
        Distribution::setConstMemberVariable(name, var);
    }
}
