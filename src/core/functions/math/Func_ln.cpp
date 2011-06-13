/**
 * @file
 * This file contains the implementation of Func_ln.
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
#include "Func_ln.h"
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
Func_ln* Func_ln::clone( void ) const {
    
    return new Func_ln( *this );
}


/** Execute function */
DAGNode* Func_ln::execute( void ) {
    
    const Real* x = static_cast<const Real*>( args[0].getValue() );
    
    return new ConstantNode( new Real( log(x->getValue()) ) );
}


/** Get argument rules */
const ArgumentRules& Func_ln::getArgumentRules( void ) const {
    
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
const VectorString& Func_ln::getClass( void ) const {
    
    static VectorString rbClass = VectorString( Func_ln_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec Func_ln::getReturnType( void ) const {
    
    return TypeSpec( Real_name );
}

