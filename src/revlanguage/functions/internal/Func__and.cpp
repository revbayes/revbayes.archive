//
//  Func__and.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 11/16/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Func__and.h"

#include "ArgumentRule.h"
#include "LogicalAndFunction.h"
#include "RbUtil.h"
#include "RlBoolean.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"

using namespace RevLanguage;

Func__and::Func__and() : TypedFunction<RlBoolean>()
{
    
}

/* Clone object */
Func__and* Func__and::clone( void ) const
{
    
    return new Func__and( *this );
}


RevBayesCore::TypedFunction<RevBayesCore::Boolean>* Func__and::createFunction( void ) const
{
    
    const RevBayesCore::TypedDagNode<RevBayesCore::Boolean>* leftVal = static_cast<const RlBoolean &>( args[0].getVariable()->getRevObject() ).getDagNode();
    const RevBayesCore::TypedDagNode<RevBayesCore::Boolean>* rightVal = static_cast<const RlBoolean &>( args[1].getVariable()->getRevObject() ).getDagNode();
    
    RevBayesCore::LogicalAndFunction *func = new RevBayesCore::LogicalAndFunction( leftVal, rightVal );
    
    return func;
}


/** Get argument rules */
const ArgumentRules& Func__and::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "", RlBoolean::getClassTypeSpec(), "The left hand side variable.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        argumentRules.push_back( new ArgumentRule( "", RlBoolean::getClassTypeSpec(), "The right hand side variable.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        rulesSet = true;
        
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func__and::getClassType(void) { 
    
    static std::string revType = "Func__and";
    
	return revType; 
}


/** Get class type spec describing type of object */
const TypeSpec& Func__and::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/** Get type spec */
const TypeSpec& Func__and::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}

