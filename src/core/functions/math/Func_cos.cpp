/**
 * @file
 * This file contains the implementation of Func_cos.
 *
 * @brief Implementation of Func_cos
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
#include "Func_cos.h"
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
const TypeSpec Func_cos::typeSpec(Func_cos_name);
const TypeSpec Func_cos::returnTypeSpec(Real_name);

/** Clone object */
Func_cos* Func_cos::clone( void ) const {
    
    return new Func_cos( *this );
}


/** Execute function */
RbPtr<RbLanguageObject> Func_cos::execute( void ) {
    
    const double x = static_cast<const Real*>( (RbLanguageObject*)(*args)[0]->getValue() )->getValue();
    
    return RbPtr<RbLanguageObject>( new Real( cos( x ) ) );
}


/** Get argument rules */
RbPtr<const ArgumentRules> Func_cos::getArgumentRules( void ) const {
    
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
const VectorString& Func_cos::getClass( void ) const {
    
    static VectorString rbClass = VectorString( Func_cos_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec& Func_cos::getReturnType( void ) const {

    return returnTypeSpec;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Func_cos::getTypeSpec(void) const {
    return typeSpec;
}

