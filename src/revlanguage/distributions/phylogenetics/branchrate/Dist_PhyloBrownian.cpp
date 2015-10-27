//
//  Dist_brownian.cpp
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-22.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//



#include "ArgumentRule.h"
#include "PhyloBrownianProcess.h"
#include "Dist_PhyloBrownian.h"
#include "ModelVector.h"
#include "Real.h"
#include "RlTimeTree.h"
#include "StochasticNode.h"
#include "Tree.h"

using namespace RevLanguage;


Dist_PhyloBrownian* Dist_PhyloBrownian::clone( void ) const {
    return new Dist_PhyloBrownian(*this);
}


RevBayesCore::PhyloBrownianProcess* Dist_PhyloBrownian::createDistribution( void ) const {
    // get the parameters

    RevBayesCore::TypedDagNode<RevBayesCore::Tree>* tau = static_cast<const TimeTree &>( tree->getRevObject() ).getDagNode();
    
    RevBayesCore::TypedDagNode<double>* s  = static_cast<const RealPos&>( sigma->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* dr  = static_cast<const Real&>( drift->getRevObject() ).getDagNode();
    
    RevBayesCore::PhyloBrownianProcess* d    = new RevBayesCore::PhyloBrownianProcess( tau, s, dr );
    
    return d;

}



/* Get Rev type of object */
const std::string& Dist_PhyloBrownian::getClassType(void) {
    
    static std::string revType = "Dist_PhyloBrownian";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_PhyloBrownian::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return revTypeSpec;
}



/** Return member rules (no members) */
const MemberRules& Dist_PhyloBrownian::getParameterRules(void) const
{
    
    static MemberRules dist;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        dist.push_back( new ArgumentRule( "tree" , TimeTree::getClassTypeSpec(), "The tree along which the continuous character evolves.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        dist.push_back( new ArgumentRule( "sigma", RealPos::getClassTypeSpec() , "The branch-length multiplier to scale the variance of the Brownian motion.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        dist.push_back( new ArgumentRule( "drift", Real::getClassTypeSpec()    , "The drift parameter of the Brownian motion.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new Real(0) ) );
        rulesSet = true;
    }
    
    return dist;
}


const TypeSpec& Dist_PhyloBrownian::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}




/** Print value for user */

 void Dist_PhyloBrownian::printValue(std::ostream& o) const {
    
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
void Dist_PhyloBrownian::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {

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

