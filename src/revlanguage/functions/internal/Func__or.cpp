//
//  Func__or.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 11/16/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Func__or.h"

#include "ArgumentRule.h"
#include "LogicalOrFunction.h"
#include "RbUtil.h"
#include "RlBoolean.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"

using namespace RevLanguage;

Func__or::Func__or() : TypedFunction<RlBoolean>() {
    
}

/* Clone object */
Func__or* Func__or::clone( void ) const {
    
    return new Func__or( *this );
}


RevBayesCore::TypedFunction<unsigned int>* Func__or::createFunction( void ) const
{
    
    const RevBayesCore::TypedDagNode<unsigned int>* leftVal = static_cast<const RlBoolean &>( args[0].getVariable()->getRevObject() ).getDagNode();
    const RevBayesCore::TypedDagNode<unsigned int>* rightVal = static_cast<const RlBoolean &>( args[1].getVariable()->getRevObject() ).getDagNode();
    
    RevBayesCore::LogicalOrFunction *func = new RevBayesCore::LogicalOrFunction( leftVal, rightVal );

    return func;
}


/** Get argument rules */
const ArgumentRules& Func__or::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "", RlBoolean::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "", RlBoolean::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func__or::getClassType(void) { 
    
    static std::string revType = "Func__or";
    
	return revType; 
}


/** Get class type spec describing type of object */
const TypeSpec& Func__or::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/** Get type spec */
const TypeSpec& Func__or::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}

