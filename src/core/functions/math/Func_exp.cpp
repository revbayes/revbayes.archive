/**
 * @file
 * This file contains the implementation of Func_exp.
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
#include "Func_exp.h"
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
const TypeSpec Func_exp::typeSpec(Func_exp_name);
const TypeSpec Func_exp::returnTypeSpec(RealPos_name);

Func_exp::Func_exp(void) : RbFunction() {
    
}

/** Copy constructor */
Func_exp::Func_exp(const Func_exp &x) : RbFunction(x) {
    
}


/** Clone object */
Func_exp* Func_exp::clone( void ) const {
    
    return new Func_exp( *this );
}


/** Execute function */
RbPtr<RbLanguageObject> Func_exp::execute( void ) {
    
    const double x = static_cast<const Real*>( (RbLanguageObject*)(*args)[0]->getValue() )->getValue();
    return RbPtr<RbLanguageObject>( new RealPos( exp( x ) ) );
}


/** Get argument rules */
RbPtr<const ArgumentRules> Func_exp::getArgumentRules( void ) const {
    
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
const VectorString& Func_exp::getClass( void ) const {
    
    static VectorString rbClass = VectorString( Func_exp_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec& Func_exp::getReturnType( void ) const {
    
    return returnTypeSpec;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Func_exp::getTypeSpec(void) const {
    return typeSpec;
}

