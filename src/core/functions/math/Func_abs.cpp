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
#include "RbUtil.h"
#include "StochasticNode.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "VectorString.h"

#include <cassert>
#include <cmath>


// Definition of the static type spec member
const TypeSpec Func_abs::typeSpec(Func_abs_name);
const TypeSpec Func_abs::returnTypeSpec(RealPos_name);

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
RbPtr<RbLanguageObject> Func_abs::execute( void ) {
    
    const double x = static_cast<const Real*>( args[0]->getValue().get() )->getValue();
    return RbPtr<RbLanguageObject>( new RealPos( fabs(x) ) );
}


/** Get argument rules */
const RbPtr<ArgumentRules> Func_abs::getArgumentRules( void ) const {
    
    static RbPtr<ArgumentRules> argumentRules( new ArgumentRules() );
    static bool          rulesSet = false;
    
    if (!rulesSet) 
    {
        argumentRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "x", Real_name ) ) );
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
const TypeSpec& Func_abs::getReturnType( void ) const {
    
    return returnTypeSpec;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Func_abs::getTypeSpec(void) const {
    return typeSpec;
}

