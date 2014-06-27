//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#include "ConstructorClade.h"
#include "RlClade.h"
#include "Vector.h"

using namespace RevLanguage;

/** default constructor */
ConstructorClade::ConstructorClade( void ) : Function( ) {
    
}


/** Clone object */
ConstructorClade* ConstructorClade::clone( void ) const {
    
    return new ConstructorClade( *this );
}


RevObject* ConstructorClade::execute() {
    
    // now allocate a new Clade
    const std::vector<std::string>& n = static_cast<const Vector<RlString> &>( args[0].getVariable()->getRevObject() ).getValue();
    RevBayesCore::Clade *c = new RevBayesCore::Clade(n,0.0);
    
    return new Clade(c);
}


/* Get argument rules */
const ArgumentRules& ConstructorClade::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) 
    {
        
        argumentRules.push_back( new ArgumentRule( "taxa", true, Vector<RlString>::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& ConstructorClade::getClassName(void) { 
    
    static std::string rbClassName = "clade constructor function";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& ConstructorClade::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get return type */
const TypeSpec& ConstructorClade::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = Clade::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& ConstructorClade::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
