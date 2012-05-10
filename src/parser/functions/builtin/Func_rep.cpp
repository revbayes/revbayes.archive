/**
 * @file
 * This file contains the implementation of Func_rep, which is
 * tused to replicate variables.
 *
 * @brief Implementation of Func_rep
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-20 10:23:29 -0700 (Fri, 20 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @package functions
 * @since Version 1.0, 2012-04-22
 *
 * $Id: Func_rep.cpp 1411 2012-04-20 17:23:29Z hoehna $
 */

#include "ConstArgumentRule.h"
#include "Func_rep.h"
#include "Natural.h"
#include "RbException.h"
#include "RbUtil.h"
#include "TypeSpec.h"



/** default constructor */
Func_rep::Func_rep( void ) : RbFunction( ) {
    
}


/** Clone object */
Func_rep* Func_rep::clone( void ) const {
    
    return new Func_rep( *this );
}


/** Execute function */
RbPtr<RbLanguageObject> Func_rep::executeFunction( const std::vector<const RbObject*>& args ) {
    
    const RbLanguageObject *var = static_cast<const RbLanguageObject*>( args[0] );
    int times = static_cast<const Natural*>( args[1] )->getValue();
    
    RbVector *repValues = new RbVector( RbLanguageObject::getClassTypeSpec() );
    for ( int i = 0; i < times; i++ ) {
        repValues->push_back( var->clone() );
    }
    
    return RbPtr<RbLanguageObject>( repValues );
}


/** Get argument rules */
const ArgumentRules& Func_rep::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if (!rulesSet) {
        
        argumentRules.push_back( new ConstArgumentRule( "x",        RbLanguageObject::getClassTypeSpec() ) );
        argumentRules.push_back( new ConstArgumentRule( "times",    Natural::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_rep::getClassName(void) { 
    
    static std::string rbClassName = "Replicate function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_rep::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_rep::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_rep::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RbVector::getClassTypeSpec();
    return returnTypeSpec;
}

