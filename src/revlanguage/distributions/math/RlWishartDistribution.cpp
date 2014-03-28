//
//  RlWishartDistribution.cpp
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-27.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#include "RlWishartDistribution.h"


#include "ArgumentRule.h"
#include "WishartDistribution.h"
#include "RlWishartDistribution.h"
#include "RealPos.h"
#include "Natural.h"
#include "RealSymmetricMatrix.h"
#include "StochasticNode.h"

using namespace RevLanguage;

WishartDistribution::WishartDistribution() : TypedDistribution<RealSymmetricMatrix>() {
    
}


WishartDistribution::~WishartDistribution() {
    
}



WishartDistribution* WishartDistribution::clone( void ) const {
    return new WishartDistribution(*this);
}


RevBayesCore::WishartDistribution* WishartDistribution::createDistribution( void ) const {
    
    // get the parameters
    RevBayesCore::TypedDagNode<RevBayesCore::PrecisionMatrix>* om = NULL;
    RevBayesCore::TypedDagNode<double>* ka = NULL;
    /*
    if (omega != NULL)  {
        om = static_cast<const RealSymmetricMatrix &>( omega->getValue() ).getValueNode();
    }
     */
    if (kappa != NULL)  {
        ka = static_cast<const RealPos&>( kappa->getValue() ).getValueNode();
    }
    
    RevBayesCore::TypedDagNode<int>* deg = static_cast<const Natural &>( df->getValue()).getValueNode();

    RevBayesCore::TypedDagNode<int>* dm = NULL;
//    int dm = -1;
    if (dim != NULL)    {
        dm = static_cast<const Natural &>( dim->getValue()).getValueNode();
//        dm = static_cast<const Natural &>( dim->getValue()).getValue();
    }
    RevBayesCore::WishartDistribution* w    =  0;
    
    if (om) {
            w = new RevBayesCore::WishartDistribution( om, deg );
    }
    else{
        if (! dm || ! ka)   {
            throw RbException("error in WishartDistribution: should specify arguments");
        }
        w = new RevBayesCore::WishartDistribution( dm, ka, deg );
    }
    return w;
}



/* Get class name of object */
const std::string& WishartDistribution::getClassName(void) {
    
    static std::string rbClassName = "Wishart distribution";
    
	return rbClassName;
}

/* Get class type spec describing type of object */
const TypeSpec& WishartDistribution::getClassTypeSpec(void) {
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass;
}




/** Return member rules (no members) */
const MemberRules& WishartDistribution::getMemberRules(void) const {
    
    static MemberRules distExpMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        
//        distExpMemberRules.push_back( new ArgumentRule( "omega", true, RealSymmetricMatrix::getClassTypeSpec() ) );
        distExpMemberRules.push_back( new ArgumentRule( "df", true, Natural::getClassTypeSpec() ) );
        distExpMemberRules.push_back( new ArgumentRule( "kappa", true, RealPos::getClassTypeSpec() ) );
        distExpMemberRules.push_back( new ArgumentRule( "dim", true, Natural::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return distExpMemberRules;
}


const TypeSpec& WishartDistribution::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void WishartDistribution::printValue(std::ostream& o) const {
    
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
void WishartDistribution::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) {
    
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
        Distribution::setConstMemberVariable(name, var);
    }
}
