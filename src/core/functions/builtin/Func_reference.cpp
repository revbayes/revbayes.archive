/**
 * @file
 * This file contains the implementation of Func_reference, which is
 * the function used to create reference of another variable.
 *
 * @brief Implementation of Func_reference
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @interface RbFunction
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id$
 */

#include "Func_reference.h"
#include "RbException.h"
#include "RbUtil.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "VectorString.h"
#include "Workspace.h"

#include <fstream>


// Definition of the static type spec member
const TypeSpec Func_reference::typeSpec(Func_reference_name);
const TypeSpec Func_reference::returnTypeSpec(RbLanguageObject_name);

/** Clone object */
Func_reference* Func_reference::clone( void ) const {
    
    return new Func_reference( *this );
}


/** Execute function */
RbLanguageObject* Func_reference::executeFunction( void ) {
    
    // reference to the original variable
    RbLanguageObject* val = (*args)[0].getValue().clone(); 
    // TODO: need to check if cloning this object will not destroy the functionality
    // It might be the case that the reference function should actually return a reference
    // to the object and not create a clone all the time.
    // Nevertheless, making an exception for exactly this case will break our memory allocation scheme.
    
    return val;
}


/** Get argument rules */
const ArgumentRules& Func_reference::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ValueRule( "variable", TypeSpec(RbLanguageObject_name) ) );
        rulesSet = true;
    }

    
    return argumentRules;
}


/** Get class vector describing type of object */
const VectorString& Func_reference::getClass( void ) const {
    
    static VectorString rbClass = VectorString( Func_reference_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec& Func_reference::getReturnType( void ) const {
    
    return returnTypeSpec;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Func_reference::getTypeSpec(void) const {
    return typeSpec;
}

