/**
 * @file
 * This file contains the implementation of Func_log.
 *
 * @brief Implementation of Func_log
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
#include "Func_log.h"
#include "RealPos.h"
#include "Real.h"
#include "RbException.h"
#include "RbUtil.h"
#include "StochasticNode.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "VectorString.h"

#include <cassert>
#include <cmath>


// Definition of the static type spec member
const TypeSpec Func_log::typeSpec(Func_log_name);
const TypeSpec Func_log::returnTypeSpec(Real_name);

/** Clone object */
Func_log* Func_log::clone( void ) const {
    
    return new Func_log( *this );
}


/** Execute function */
RbLanguageObject* Func_log::executeFunction( void ) {
    
    const double a = static_cast<const RealPos*>( (*args)[0]->getValue() )->getValue();
    const double b = static_cast<const RealPos*>( (*args)[1]->getValue() )->getValue();
    
    return new Real( log10(a)/log10(b) );
}


/** Get argument rules */
const ArgumentRules* Func_log::getArgumentRules( void ) const {
    
    static ArgumentRules* argumentRules = new ArgumentRules();
    static bool          rulesSet = false;
    
    if (!rulesSet) 
    {
        argumentRules->push_back( new ValueRule( "x", RealPos_name ) );
        argumentRules->push_back( new ValueRule( "base", RealPos_name, new RealPos(10.0) ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class vector describing type of object */
const VectorString& Func_log::getClass( void ) const {
    
    static VectorString rbClass = VectorString( Func_log_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec& Func_log::getReturnType( void ) const {
    
    return returnTypeSpec;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Func_log::getTypeSpec(void) const {
    return typeSpec;
}

