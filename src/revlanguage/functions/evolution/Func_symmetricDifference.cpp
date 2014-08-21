//
//  Func_symmetricDifference.cpp
//  RevBayesCore
//
//  Created by Bastien Boussau on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Func_symmetricDifference.h"
#include "ModelVector.h"
#include "RlTimeTree.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "Topology.h"
#include "SymmetricDifferenceStatistic.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_symmetricDifference::Func_symmetricDifference( void ) : Function( ) {
    
}


/** Clone object */
Func_symmetricDifference* Func_symmetricDifference::clone( void ) const {
    
    return new Func_symmetricDifference( *this );
}


RevPtr<Variable> Func_symmetricDifference::execute() {
    
    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree>* tau = static_cast<const TimeTree&>( this->args[0].getVariable()->getRevObject() ).getDagNode();
     RevBayesCore::TypedDagNode<RevBayesCore::TimeTree>* tau2 = static_cast<const TimeTree&>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::SymmetricDifferenceStatistic* f = new RevBayesCore::SymmetricDifferenceStatistic( tau, tau2 );
    
    DeterministicNode<double> *detNode = new DeterministicNode<double>("", f, this->clone());
    
    RealPos* value = new RealPos( detNode );
    
    return new Variable( value );
}


/* Get argument rules */
const ArgumentRules& Func_symmetricDifference::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "tree1", true, TimeTree::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "tree2", true, TimeTree::getClassTypeSpec() ) );

        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_symmetricDifference::getClassType(void) {
    
    static std::string revType = "Func_symmetricDifference";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_symmetricDifference::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/* Get return type */
const TypeSpec& Func_symmetricDifference::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RealPos::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_symmetricDifference::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
