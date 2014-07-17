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

Func__or::Func__or() : Function() {
    
}

/* Clone object */
Func__or* Func__or::clone( void ) const {
    
    return new Func__or( *this );
}


/** Execute function: We rely on getValue or overloaded push_back to provide functionality */
RevObject* Func__or::execute( void ) {
    
    const RevBayesCore::TypedDagNode<bool>* leftVal = static_cast<const RlBoolean &>( args[0].getVariable()->getRevObject() ).getDagNode();
    const RevBayesCore::TypedDagNode<bool>* rightVal = static_cast<const RlBoolean &>( args[1].getVariable()->getRevObject() ).getDagNode();
    
    RevBayesCore::LogicalOrFunction *func = new RevBayesCore::LogicalOrFunction( leftVal, rightVal );

    DeterministicNode<bool> *detNode = new DeterministicNode<bool>("", func, this->clone());
    
    RlBoolean *theBool = new RlBoolean( detNode );
    
    return theBool;
    
}


/** Get argument rules */
const ArgumentRules& Func__or::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "", true, RlBoolean::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "", true, RlBoolean::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
const std::string& Func__or::getClassName(void) { 
    
    static std::string rbClassName = "Func__or";
    
	return rbClassName; 
}


/** Get class type spec describing type of object */
const RevLanguage::TypeSpec& Func__or::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get type spec */
const TypeSpec& Func__or::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func__or::getReturnType( void ) const {
    
    return RlBoolean::getClassTypeSpec();
}

