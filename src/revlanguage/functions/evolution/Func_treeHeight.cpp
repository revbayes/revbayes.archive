//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Func_treeHeight.h"
#include "ModelVector.h"
#include "RlTimeTree.h"
#include "RateMatrix.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "Topology.h"
#include "TreeHeightStatistic.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_treeHeight::Func_treeHeight( void ) : Function( ) {
    
}


/** Clone object */
Func_treeHeight* Func_treeHeight::clone( void ) const {
    
    return new Func_treeHeight( *this );
}


RevPtr<Variable> Func_treeHeight::execute() {
    
    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree>* tau = static_cast<const TimeTree&>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TreeHeightStatistic* f = new RevBayesCore::TreeHeightStatistic( tau );
    
    DeterministicNode<double> *detNode = new DeterministicNode<double>("", f, this->clone());
    
    RealPos* value = new RealPos( detNode );
    
    return new Variable( value );
}


/* Get argument rules */
const ArgumentRules& Func_treeHeight::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) 
    {
        
        argumentRules.push_back( new ArgumentRule( "tree", true, TimeTree::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_treeHeight::getClassType(void) { 
    
    static std::string revType = "Func_treeHeight";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_treeHeight::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/* Get return type */
const TypeSpec& Func_treeHeight::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RealPos::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_treeHeight::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
