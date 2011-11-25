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
#include "RbUtil.h"
#include "StochasticNode.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "VectorString.h"

#include <cassert>
#include <cmath>


// Definition of the static type spec member
const TypeSpec Func_ln::typeSpec(Func_ln_name);
const TypeSpec Func_ln::returnTypeSpec(Real_name);

/** Clone object */
Func_ln* Func_ln::clone( void ) const {
    
    return new Func_ln( *this );
}


/** Execute function */
RbPtr<RbLanguageObject> Func_ln::executeFunction( void ) {
    
    const double x = static_cast<const Real*>( (RbObject*)(*args)[0]->getValue() )->getValue();
    
    return RbPtr<RbLanguageObject>( new Real( log(x) ) );
}


/** Get argument rules */
RbPtr<const ArgumentRules> Func_ln::getArgumentRules( void ) const {
    
    static RbPtr<ArgumentRules> argumentRules( new ArgumentRules() );
    static bool          rulesSet = false;
    
    if (!rulesSet) 
    {
        argumentRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "x", RealPos_name ) ) );
        rulesSet = true;
    }
    
    return RbPtr<const ArgumentRules>( argumentRules );
}


/** Get class vector describing type of object */
const VectorString& Func_ln::getClass( void ) const {
    
    static VectorString rbClass = VectorString( Func_ln_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec& Func_ln::getReturnType( void ) const {
    
    return returnTypeSpec;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Func_ln::getTypeSpec(void) const {
    return typeSpec;
}

