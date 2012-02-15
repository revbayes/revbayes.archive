/**
 * @file
 * This file contains the implementation of Func_structure, which is used
 * to print info about the structure of a variable.
 *
 * @brief Implementation of Func_structure
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
#include "Func_structure.h"
#include "Integer.h"
#include "RbException.h"
#include "RbNullObject.h"
#include "RbUtil.h"
#include "StochasticNode.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "VectorInteger.h"
#include "VectorString.h"

#include <cassert>


// Definition of the static type spec member
const TypeSpec Func_structure::typeSpec(Func_structure_name);
const TypeSpec Func_structure::returnTypeSpec(RbVoid_name);

/** Clone object */
Func_structure* Func_structure::clone( void ) const {

    return new Func_structure( *this );
}


/** Execute function */
const RbLanguageObject& Func_structure::executeFunction( void ) {

    (*args)[0].getDagNode()->printStruct( std::cout );

    return RbNullObject::getInstance();
}


/** Get argument rules */
const ArgumentRules& Func_structure::getArgumentRules( void ) const {

    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;

    if ( !rulesSet ) {

        argumentRules.push_back( new ValueRule( "variable",   RbObject_name ) );
        rulesSet = true;
    }

    return argumentRules;
}


/** Get class vector describing type of object */
const VectorString& Func_structure::getClass( void ) const {

    static VectorString rbClass = VectorString( Func_structure_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec& Func_structure::getReturnType( void ) const {

    return returnTypeSpec;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Func_structure::getTypeSpec(void) const {
    return typeSpec;
}

