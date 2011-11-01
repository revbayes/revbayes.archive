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
#include "RbUtil.h"
#include "StochasticNode.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "VectorString.h"

#include <cassert>
#include <cmath>


// Definition of the static type spec member
const TypeSpec Func_power::typeSpec(Func_power_name);
const TypeSpec Func_power::returnTypeSpec(Real_name);

/** Clone object */
Func_power* Func_power::clone( void ) const {
    
    return new Func_power( *this );
}


/** Execute function */
RbPtr<RbLanguageObject> Func_power::execute( void ) {
    
    const double a = static_cast<const Real*>( (const RbLanguageObject*)(*args)[0]->getValue() )->getValue();
    const double b = static_cast<const Real*>( (const RbLanguageObject*)(*args)[1]->getValue() )->getValue();
    
    return RbPtr<RbLanguageObject>( new Real( pow(a,b) ) );
}


/** Get argument rules */
RbPtr<const ArgumentRules> Func_power::getArgumentRules( void ) const {
    
    static RbPtr<ArgumentRules> argumentRules( new ArgumentRules() );
    static bool          rulesSet = false;
    
    if (!rulesSet) 
    {
        argumentRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "a", Real_name ) ) );
        argumentRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "b", Real_name ) ) );
        rulesSet = true;
    }
    
    return RbPtr<const ArgumentRules>( argumentRules );
}


/** Get class vector describing type of object */
const VectorString& Func_power::getClass( void ) const {
    
    static VectorString rbClass = VectorString( Func_power_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec& Func_power::getReturnType( void ) const {
    
    return returnTypeSpec;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Func_power::getTypeSpec(void) const {
    return typeSpec;
}

