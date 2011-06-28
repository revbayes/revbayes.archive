/**
 * @file
 * This file contains the implementation of Func_sqrt, which 
 * calculates the square root of a number.
 *
 * @brief Implementation of Func_sqrt
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

#include "ConstantNode.h"
#include "DAGNode.h"
#include "DeterministicNode.h"
#include "Func_sqrt.h"
#include "RealPos.h"
#include "Real.h"
#include "RbException.h"
#include "RbNames.h"
#include "StochasticNode.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "VectorString.h"

#include <cassert>
#include <cmath>

/** Clone object */
Func_sqrt* Func_sqrt::clone( void ) const {

    return new Func_sqrt( *this );
}


/** Execute function */
DAGNode* Func_sqrt::execute( void ) {

    const Real* x = static_cast<const Real*>( args[0].getValue() );

    if ( *x < 0.0 )
		throw RbException( "Square root of negative number. RevBayes does not (yet) support complex numbers" );

    return ( new RealPos( sqrt( *x ) ) )->wrapIntoVariable();
}


/** Get argument rules */
const ArgumentRules& Func_sqrt::getArgumentRules( void ) const {

    static ArgumentRules argumentRules;
    static bool          rulesSet = false;

    if (!rulesSet) 
		{
        argumentRules.push_back( new ValueRule( "x", RealPos_name ) );
        rulesSet = true;
		}

    return argumentRules;
}


/** Get class vector describing type of object */
const VectorString& Func_sqrt::getClass( void ) const {

    static VectorString rbClass = VectorString( Func_sqrt_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec Func_sqrt::getReturnType( void ) const {

    return TypeSpec( RealPos_name );
}

