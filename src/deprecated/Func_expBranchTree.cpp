//
//  Func_expBranchTree.cpp
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-24.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#include "ExponentialBranchTree.h"
#include "Func_expBranchTree.h"

#include "Integer.h"
#include "MatrixReal.h"
#include "ModelVector.h"
#include "Natural.h"
#include "RealPos.h"
#include "RlMultivariateRealNodeValTree.h"
#include "RlRealNodeValTree.h"
#include "RlTimeTree.h"

using namespace RevLanguage;


Func_expBranchTree* Func_expBranchTree::clone( void ) const {

    return new Func_expBranchTree(*this);
}

/*
RevBayesCore::ExponentialBranchTree* ExponentialBranchTree::createFunction(void) const {
    
    // todo: implement this or find better solution (Sebastian)
    throw RbException("Missing implemention in ExponentialBranchTree::createFunction");
    
    return NULL;
}
*/

/* Get argument rules */
const ArgumentRules& Func_expBranchTree::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
 
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "tree"      , RevLanguage::TimeTree::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );

        std::vector<TypeSpec> types;
        types.push_back(MultivariateRealNodeValTree::getClassTypeSpec());
        types.push_back(RealNodeValTree::getClassTypeSpec());
        argumentRules.push_back( new ArgumentRule( "process"   , types                      , ArgumentRule::BY_CONSTANT_REFERENCE) );
        argumentRules.push_back( new ArgumentRule( "offset"    , Real::getClassTypeSpec()   , ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new Real(0) ) );
        argumentRules.push_back( new ArgumentRule( "traitindex", Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new Natural(0)) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}



/* Get Rev type of object */
const std::string& Func_expBranchTree::getClassType(void) {
    
    static std::string revType = "Func_expBranchTree";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_expBranchTree::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}



const TypeSpec& Func_expBranchTree::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


RevBayesCore::TypedFunction< RevBayesCore::RbVector<double> >* Func_expBranchTree::createFunction( void ) const
{
    
    
    RevBayesCore::TypedDagNode< RevBayesCore::TimeTree >* tau = static_cast<const TimeTree &>( args[0].getVariable()->getRevObject() ).getDagNode();

    RevBayesCore::TypedDagNode< RevBayesCore::MultivariateRealNodeContainer >* mvprocess = 0;
    RevBayesCore::TypedDagNode< RevBayesCore::RealNodeContainer >* uniprocess = 0;
    
    const MultivariateRealNodeValTree* mv = dynamic_cast<const MultivariateRealNodeValTree *>( &(args[1].getVariable()->getRevObject()) ); 
    const RealNodeValTree* uni = dynamic_cast<const RealNodeValTree *>( &(args[1].getVariable()->getRevObject()) ); 
    if (mv) {
        mvprocess = static_cast<const MultivariateRealNodeValTree &>( args[1].getVariable()->getRevObject() ).getDagNode();
    }
    else    {
        if (! uni)  {
            std::cerr << "error: dcast into univariate did not work\n";
            exit(1);
        }
        uniprocess = static_cast<const RealNodeValTree &> (args[1].getVariable()->getRevObject()).getDagNode();
    }

    RevBayesCore::TypedDagNode< double >* offset = static_cast<const Real &>( args[2].getVariable()->getRevObject() ).getDagNode();

    RevBayesCore::TypedDagNode< int >* traitindex = static_cast<const Integer &> (args[3].getVariable()->getRevObject()).getDagNode();
    
    RevBayesCore::ExponentialBranchTree* result = new RevBayesCore::ExponentialBranchTree( tau, mvprocess, uniprocess, offset, traitindex );

    return result;
}


void Func_expBranchTree::printValue(std::ostream& o) const {
    
    o << " expbranchtree(";
   
    o << "tree=";
    if ( argsProcessed && args[0].getVariable() != NULL ) {
        o << args[0].getVariable()->getName();
    } else {
        o << "?";
    }
    o << ", ";
    
    o << "process=";
    if ( argsProcessed && args[1].getVariable() != NULL ) {
        o << args[1].getVariable()->getName();
    } else {
        o << "?";
    }
    o << ", ";
    
    o << "offset=";
    if ( argsProcessed && args[2].getVariable() != NULL ) {
        o << args[2].getVariable()->getName();
    } else {
        o << "?";
    }
    o << ", ";
    
    o << "traitindex=";
    if ( argsProcessed && args[3].getVariable() != NULL ) {
        o << args[3].getVariable()->getName();
    } else {
        o << "?";
    }
    o << ")";
}



