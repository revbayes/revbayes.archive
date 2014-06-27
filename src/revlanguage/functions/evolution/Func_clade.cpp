//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#include "Func_clade.h"
#include "RlClade.h"
#include "Vector.h"

using namespace RevLanguage;

/** default constructor */
Func_clade::Func_clade( void ) : Function( ) {
    
}


/** Clone object */
Func_clade* Func_clade::clone( void ) const {
    
    return new Func_clade( *this );
}


RevObject* Func_clade::execute() {
    
    // now allocate a new Clade
    const std::vector<std::string>& n = static_cast<const Vector<RlString> &>( args[0].getVariable()->getRevObject() ).getValue();
    RevBayesCore::Clade *c = new RevBayesCore::Clade(n,0.0);
    
    return new Clade(c);
}


/* Get argument rules */
const ArgumentRules& Func_clade::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) 
    {
        
        argumentRules.push_back( new ArgumentRule( "taxa", true, Vector<RlString>::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_clade::getClassName(void) { 
    
    static std::string rbClassName = "Func_clade";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_clade::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get return type */
const TypeSpec& Func_clade::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = Clade::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_clade::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
