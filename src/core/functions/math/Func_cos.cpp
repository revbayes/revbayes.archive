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

#include <cassert>
#include <cmath>


/** Clone object */
Func_cos* Func_cos::clone( void ) const {
    
    return new Func_cos( *this );
}


/** Execute function */
const RbLanguageObject& Func_cos::executeFunction( void ) {
    
    const double d = static_cast<const Real&>( x->getValue() ).getValue();
    value.setValue( cos( d ) );
    return value;
}


/** Get argument rules */
const ArgumentRules& Func_cos::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if (!rulesSet) 
    {
        argumentRules.push_back( new ValueRule( "x", Real::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_cos::getClassName(void) { 
    
    static std::string rbClassName = "Cos function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_cos::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_cos::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_cos::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = Real::getClassTypeSpec();
    return returnTypeSpec;
}


/** We catch here the setting of the argument variables to store our parameters. */
void Func_cos::setArgumentVariable(std::string const &name, const Variable* var) {
    
    if ( name == "x" ) {
        x = var;
    }
    else {
        RbFunction::setArgumentVariable(name, var);
    }
}

