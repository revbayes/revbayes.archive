//
//  Func_treeScale.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 2/6/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#include "Func_treeScale.h"
#include "ModelVector.h"
#include "Real.h"
#include "RealPos.h"
#include "RlTimeTree.h"
#include "RlDeterministicNode.h"
#include "TreeScaleFunction.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_treeScale::Func_treeScale( void ) : TypedFunction<TimeTree>( ) {
    
}


/** Clone object */
Func_treeScale* Func_treeScale::clone( void ) const {
    
    return new Func_treeScale( *this );
}


RevBayesCore::TypedFunction<RevBayesCore::TimeTree>* Func_treeScale::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree>* tau = static_cast<const TimeTree&>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* scale               = static_cast<const RealPos &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TreeScaleFunction* f = new RevBayesCore::TreeScaleFunction( tau, scale );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_treeScale::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "tree",    TimeTree::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "scale",   RealPos::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_treeScale::getClassType(void) {
    
    static std::string revType = "Func_treeScale";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_treeScale::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


const TypeSpec& Func_treeScale::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
