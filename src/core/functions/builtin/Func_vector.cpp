//
//  Func_vector.cpp
//  RevBayes
//
//  Created by Sebastian Hoehna on 5/16/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Func_vector.h"
#include "ConstArgumentRule.h"
#include "Ellipsis.h"
#include "RbUtil.h"
#include "RbVector.h"
#include "TypeSpec.h"


/* Default constructor */
Func_vector::Func_vector( const RbPtr<RbObject> &valueType ) : valType(valueType),  returnTypeSpec( TypeSpec( RbVector::getClassTypeSpec(), new TypeSpec( valueType->getTypeSpec() ) ) ) {
    
    argumentRules.push_back( new ConstArgumentRule( "", valType->getTypeSpec() ) );
    argumentRules.push_back( new Ellipsis (     valType->getTypeSpec() ) );
    
}

/* Clone object */
Func_vector* Func_vector::clone( void ) const {
    
    return new Func_vector( *this );
}


/** Execute function: We rely on getValue and overloaded push_back to provide functionality */
RbPtr<RbLanguageObject> Func_vector::executeFunction( const std::vector<const RbObject*>& args ) {
    
    RbVector *theVector = new RbVector( valType->getTypeSpec() );
    for ( size_t i = 0; i < args.size(); i++ )
        theVector->push_back( args[i]->clone() );
    
    return theVector;
}


/** Get argument rules */
const ArgumentRules& Func_vector::getArgumentRules( void ) const {
    
    return argumentRules;
}



/** Get class name of object */
const std::string& Func_vector::getClassName(void) { 
    
    static std::string rbClassName = "Func_vector<>";
    
	return rbClassName; 
}


/** Get class type spec describing type of object */
const TypeSpec& Func_vector::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get type spec */
const TypeSpec& Func_vector::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_vector::getReturnType( void ) const {
    
    return returnTypeSpec;
}
