/**
 * @file
 * This file contains the implementation of Func_unique, which is
 * the function used to count the number of unique values in a vector.
 *
 * @brief Implementation of Func_unique
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id$
 */

#include "DAGNode.h"
#include "DeterministicNode.h"
#include "Func_unique.h"
#include "Integer.h"
#include "RbException.h"
#include "RbUtil.h"
#include "StochasticNode.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "VectorInteger.h"
#include "VectorString.h"
#include "VectorReal.h"

#include <cassert>

/** Clone object */
Func_unique* Func_unique::clone( void ) const {
	
    return new Func_unique( *this );
}


/** Execute function */
RbLanguageObject* Func_unique::execute( void ) {
	
	//VectorReal* tempVec = new VectorReal;
	
	return NULL;
}


/** Get argument rules */
const ArgumentRules& Func_unique::getArgumentRules( void ) const {
	
    static ArgumentRules argumentRules;
    static bool          rulesSet = false;
	
    if ( !rulesSet ) {
		
        argumentRules.push_back( new ValueRule( "", VectorReal_name ) );
        rulesSet = true;
    }
	
    return argumentRules;
}


/** Get class vector describing type of object */
const VectorString& Func_unique::getClass( void ) const {
	
    static VectorString rbClass = VectorString( Func_unique_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec Func_unique::getReturnType( void ) const {
	
    return TypeSpec( RbVoid_name );
}

