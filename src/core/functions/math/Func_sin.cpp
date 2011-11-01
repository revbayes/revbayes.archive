/**
 * @file
 * This file contains the implementation of Func_sin.
 *
 * @brief Implementation of Func_sin
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
#include "Func_sin.h"
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
const TypeSpec Func_sin::typeSpec(Func_sin_name);
const TypeSpec Func_sin::returnTypeSpec(Real_name);

/** Clone object */
Func_sin* Func_sin::clone( void ) const {
    
    return new Func_sin( *this );
}


/** Execute function */
RbPtr<RbLanguageObject> Func_sin::execute( void ) {
    
    const double x = static_cast<const Real*>( (const RbLanguageObject*)(*args)[0]->getValue() )->getValue();
    
    return RbPtr<RbLanguageObject>( new RealPos( sin(x) ) );
}


/** Get argument rules */
RbPtr<const ArgumentRules> Func_sin::getArgumentRules( void ) const {
    
    static RbPtr<ArgumentRules> argumentRules( new ArgumentRules() );
    static bool          rulesSet = false;
    
    if (!rulesSet) 
    {
        argumentRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "x", Real_name ) ) );
        rulesSet = true;
    }
    
    return RbPtr<const ArgumentRules>( argumentRules );
}


/** Get class vector describing type of object */
const VectorString& Func_sin::getClass( void ) const {
    
    static VectorString rbClass = VectorString( Func_sin_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec& Func_sin::getReturnType( void ) const {
    
    return returnTypeSpec;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Func_sin::getTypeSpec(void) const {
    return typeSpec;
}

