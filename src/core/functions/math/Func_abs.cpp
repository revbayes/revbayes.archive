/**
 * @file
 * This file contains the implementation of Func_abs.
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
 * $Id$
 */

#include "ConstantNode.h"
#include "DAGNode.h"
#include "DeterministicNode.h"
#include "Func_abs.h"
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

Func_abs::Func_abs(void) : RbFunction() {
    
}

/** Copy constructor */
Func_abs::Func_abs(const Func_abs &x) : RbFunction(x) {
    
}


/** Clone object */
Func_abs* Func_abs::clone( void ) const {
    
    return new Func_abs( *this );
}


/** Execute function */
RbLanguageObject* Func_abs::execute( void ) {
    
    const Real* x = static_cast<const Real*>( args[0].getValue() );
    return ( new RealPos( abs(x->getValue()) ) );
}


/** Get argument rules */
const ArgumentRules& Func_abs::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules;
    static bool          rulesSet = false;
    
    if (!rulesSet) 
    {
        argumentRules.push_back( new ValueRule( "x", Real_name ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class vector describing type of object */
const VectorString& Func_abs::getClass( void ) const {
    
    static VectorString rbClass = VectorString( Func_abs_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec Func_abs::getReturnType( void ) const {
    
    return TypeSpec( RealPos_name );
}

