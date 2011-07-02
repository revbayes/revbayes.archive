/**
 * @file
 * This file contains the implementation of Func_power.
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
#include "Func_power.h"
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
Func_power* Func_power::clone( void ) const {
    
    return new Func_power( *this );
}


/** Execute function */
DAGNode* Func_power::execute( void ) {
    
    const Real* a = static_cast<const Real*>( args[0].getValue() );
    const Real* b = static_cast<const Real*>( args[1].getValue() );
    
    return ( new Real( pow(a->getValue(),b->getValue()) ) )->wrapIntoVariable();
}


/** Get argument rules */
const ArgumentRules& Func_power::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules;
    static bool          rulesSet = false;
    
    if (!rulesSet) 
    {
        argumentRules.push_back( new ValueRule( "a", Real_name ) );
        argumentRules.push_back( new ValueRule( "b", Real_name ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class vector describing type of object */
const VectorString& Func_power::getClass( void ) const {
    
    static VectorString rbClass = VectorString( Func_power_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec Func_power::getReturnType( void ) const {
    
    return TypeSpec( Real_name );
}

