//
//  Dist_wishart.cpp
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-27.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#include "ArgumentRule.h"
#include "Dist_wishart.h"
#include "Natural.h"
#include "RealPos.h"
#include "RealSymmetricMatrix.h"
#include "StochasticNode.h"
#include "WishartDistribution.h"

using namespace RevLanguage;

Dist_wishart::Dist_wishart() : TypedDistribution<RealSymmetricMatrix>() {
    
}


Dist_wishart::~Dist_wishart() {
    
}



Dist_wishart* Dist_wishart::clone( void ) const {
    return new Dist_wishart(*this);
}


RevBayesCore::WishartDistribution* Dist_wishart::createDistribution( void ) const {
    
    // get the parameters
    RevBayesCore::TypedDagNode<RevBayesCore::MatrixRealSymmetric>* om = NULL;
    RevBayesCore::TypedDagNode<double>* ka = NULL;
    /*
    if (omega != NULL)  {
        om = static_cast<const RealSymmetricMatrix &>( omega->getRevObject() ).getDagNode();
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
    RevBayesCore::WishartDistribution* w    =  0;
    
    if (om) {
            w = new RevBayesCore::WishartDistribution( om, deg );
    }
    else{
        if (! dm || ! ka)   {
            throw RbException("error in Dist_wishart: should specify arguments");
        }
        w = new RevBayesCore::WishartDistribution( dm, ka, deg );
    }
    return w;
}



/* Get Rev type of object */
const std::string& Dist_wishart::getClassType(void) {
    
    static std::string revType = "Dist_wishart";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_wishart::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return revTypeSpec;
}




/** Return member rules (no members) */
const MemberRules& Dist_wishart::getParameterRules(void) const {
    
    static MemberRules distMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        
//        distExpMemberRules.push_back( new ArgumentRule( "omega", true, RealSymmetricMatrix::getClassTypeSpec() ) );
        distMemberRules.push_back( new ArgumentRule( "df"   , Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        distMemberRules.push_back( new ArgumentRule( "kappa", RealPos::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        distMemberRules.push_back( new ArgumentRule( "dim"  , Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        
        rulesSet = true;
    }
    
    return distMemberRules;
}


const TypeSpec& Dist_wishart::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void Dist_wishart::printValue(std::ostream& o) const {
    
    o << " Wishart(omega=";
/*
    if ( omega != NULL ) {
        o << omega->getName();
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
void Dist_wishart::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "omega" ) {
//        omega = var;
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
        Distribution::setConstParameter(name, var);
    }
}
