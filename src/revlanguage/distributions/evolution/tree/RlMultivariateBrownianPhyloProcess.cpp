//
//  RlWhiteNoiseProcess.cpp
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-22.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//



#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "MultivariateBrownianPhyloProcess.h"
#include "RlMultivariateBrownianPhyloProcess.h"
#include "Real.h"
#include "RlTimeTree.h"
#include "TimeTree.h"
#include "Real.h"
#include "StochasticNode.h"
#include "PrecisionMatrix.h"
#include "RealSymmetricMatrix.h"
#include "Vector.h"

using namespace RevLanguage;


MultivariateBrownianPhyloProcess* MultivariateBrownianPhyloProcess::clone( void ) const {
    return new MultivariateBrownianPhyloProcess(*this);
}


RevBayesCore::MultivariateBrownianPhyloProcess* MultivariateBrownianPhyloProcess::createDistribution( void ) const {
    // get the parameters

    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree>* tau = static_cast<const TimeTree &>( tree->getValue() ).getValueNode();
    
    RevBayesCore::TypedDagNode<RevBayesCore::PrecisionMatrix>* om  = static_cast<const RealSymmetricMatrix&>( omega->getValue() ).getValueNode();
    RevBayesCore::TypedDagNode<std::vector<double> >* r  = static_cast<const Vector<Real>&>( rootval->getValue() ).getValueNode();
    
    RevBayesCore::MultivariateBrownianPhyloProcess* process    = new RevBayesCore::MultivariateBrownianPhyloProcess( tau, om, r );
    
    return process;

}



/* Get class name of object */
const std::string& MultivariateBrownianPhyloProcess::getClassName(void) {
    
    static std::string rbClassName = "MultivariateBrownianPhyloProcess";
    
	return rbClassName;
}

/* Get class type spec describing type of object */
const TypeSpec& MultivariateBrownianPhyloProcess::getClassTypeSpec(void) {
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass;
}



/** Return member rules (no members) */
const MemberRules& MultivariateBrownianPhyloProcess::getMemberRules(void) const {
    
    static MemberRules dist;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        dist.push_back( new ArgumentRule( "tree" , true, TimeTree::getClassTypeSpec() ) );
        dist.push_back( new ArgumentRule( "omega", true, RealSymmetricMatrix::getClassTypeSpec() ) );
        dist.push_back( new ArgumentRule( "rootval", true, Vector<Real>::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return dist;
}


const TypeSpec& MultivariateBrownianPhyloProcess::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}




/** Print value for user */

 void MultivariateBrownianPhyloProcess::printValue(std::ostream& o) const {
    
    o << " MultivariateBrownian(";
    
    o << "tau=";
    if ( tree != NULL ) {
        o << tree->getName();
    } else {
        o << "?";
    }

     o << ",";
     
     o << "omega=";
     if ( omega != NULL ) {
         o << omega->getName();
     } else {
         o << "?";
     }

     o << ",";
     
     o << "rootval=";
     if ( rootval != NULL ) {
         o << rootval->getName();
     } else {
         o << "?";
     }

     o << ")";
}



/** Set a member variable */
void MultivariateBrownianPhyloProcess::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) {

    if ( name == "tree" )
    {
        tree = var;
    }
    else if ( name == "omega" )
    {
        omega = var;
    }
    else if ( name == "rootval" )
    {
        rootval = var;
    }
    else {
        Distribution::setConstMemberVariable(name, var);
    }
}

