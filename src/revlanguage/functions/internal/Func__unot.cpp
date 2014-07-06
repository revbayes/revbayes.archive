//
//  Func__unot.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 11/16/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Func__unot.h"

#include "ArgumentRule.h"
#include "LogicalUnotFunction.h"
#include "RbUtil.h"
#include "RlBoolean.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"

using namespace RevLanguage;

Func__unot::Func__unot() : Function() {
    
}

/* Clone object */
Func__unot* Func__unot::clone( void ) const {
    
    return new Func__unot( *this );
}


/** Execute function: We rely on getValue unot overloaded push_back to provide functionality */
RevObject* Func__unot::execute( void ) {
    
    const RevBayesCore::TypedDagNode<bool>* val = static_cast<const RlBoolean &>( args[0].getVariable()->getRevObject() ).getDagNode();
    
    RevBayesCore::LogicalUnotFunction *func = new RevBayesCore::LogicalUnotFunction( val );

    DeterministicNode<bool> *detNode = new DeterministicNode<bool>("", func, this->clone());
    
    RlBoolean *theBool = new RlBoolean( detNode );
    
    return theBool;
    
}


/** Get argument rules */
const ArgumentRules& Func__unot::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "", true, RlBoolean::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
const std::string& Func__unot::getClassName(void) { 
    
    static std::string rbClassName = "Func__unot";
    
	return rbClassName; 
}


/** Get class type spec describing type of object */
const RevLanguage::TypeSpec& Func__unot::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get type spec */
const TypeSpec& Func__unot::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func__unot::getReturnType( void ) const {
    
    return RlBoolean::getClassTypeSpec();
}

