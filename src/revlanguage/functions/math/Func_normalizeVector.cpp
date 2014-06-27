//
//  Func_normalizeVector.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 9/8/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Func_normalizeVector.h"
#include "ArgumentRule.h"
#include "DeterministicNode.h"
#include "Ellipsis.h"
#include "RbUtil.h"
#include "RealPos.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"
#include "NormalizeVectorFunction.h"
#include "Vector.h"


using namespace RevLanguage;

Func_normalizeVector::Func_normalizeVector() : Function() {
    
}

/* Clone object */
Func_normalizeVector* Func_normalizeVector::clone( void ) const {
    
    return new Func_normalizeVector( *this );
}


/** Execute function: We rely on getValue and overloaded push_back to provide functionality */
RevObject* Func_normalizeVector::execute( void ) {
    
    const RevBayesCore::TypedDagNode< std::vector<double> > *params = static_cast< Vector<RealPos> & >( args[0].getVariable()->getRevObject() ).getDagNode();
    
    RevBayesCore::NormalizeVectorFunction *func = new RevBayesCore::NormalizeVectorFunction( params );
    RevBayesCore::TypedDagNode<std::vector<double> > *detNode = new RevBayesCore::DeterministicNode<std::vector<double> >("", func);
    Vector<RealPos> *theNormalizedVector = new Vector<RealPos>( detNode );
    
    return theNormalizedVector;
}


/** Get argument rules */
const ArgumentRules& Func_normalizeVector::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "x", true, Vector<RealPos>::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_normalizeVector::getClassName(void) { 
    
    static std::string rbClassName = "Func_normalizeVector";
    
	return rbClassName; 
}


/** Get class type spec describing type of object */
const RevLanguage::TypeSpec& Func_normalizeVector::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( "Func_normalizeVector", new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get type spec */
const TypeSpec& Func_normalizeVector::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_normalizeVector::getReturnType( void ) const {
    
    return Vector<RealPos>::getClassTypeSpec();
}
