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
RbPtr<RbLanguageObject> Func_reference::execute( void ) {
    
    // reference to the original variable
    RbPtr<RbLanguageObject> val = (*args)[0]->getValue();
    
    return val;
}


/** Get argument rules */
RbPtr<const ArgumentRules> Func_reference::getArgumentRules( void ) const {
    
    static RbPtr<ArgumentRules> argumentRules( new ArgumentRules() );
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "variable", TypeSpec(RbLanguageObject_name) ) ) );
        rulesSet = true;
    }

    
    return RbPtr<const ArgumentRules>( argumentRules );
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

