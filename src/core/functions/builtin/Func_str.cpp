/**
 * @file
 * This file contains the implementation of Func_str, which is used
 * to print info about the structure of a variable.
 *
 * @brief Implementation of Func_str
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id:$
 */

#include "DAGNode.h"
#include "DeterministicNode.h"
#include "Func_str.h"
#include "Integer.h"
#include "LookupNode.h"
#include "RbException.h"
#include "RbNames.h"
#include "ReferenceRule.h"
#include "StochasticNode.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "VectorInteger.h"
#include "VectorString.h"

#include <cassert>

/** Clone object */
Func_str* Func_str::clone( void ) const {

    return new Func_str( *this );
}


/** Execute function */
DAGNode* Func_str::execute( void ) {

    args[0].getReference()->printStruct( std::cout );

    return NULL;
}


/** Get argument rules */
const ArgumentRules& Func_str::getArgumentRules( void ) const {

    static ArgumentRules argumentRules;
    static bool          rulesSet = false;

    if ( !rulesSet ) {

        argumentRules.push_back( new ReferenceRule( "variable",   RbObject_name ));
        rulesSet = true;
    }

    return argumentRules;
}


/** Get class vector describing type of object */
const VectorString& Func_str::getClass( void ) const {

    static VectorString rbClass = VectorString( Func_str_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec Func_str::getReturnType( void ) const {

    return TypeSpec( RbVoid_name );
}

