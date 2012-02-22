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


/** Clone object */
Func_log* Func_log::clone( void ) const {
    
    return new Func_log( *this );
}


/** Execute function */
const RbLanguageObject& Func_log::executeFunction( void ) {
    
    const double a = static_cast<const RealPos&>( x->getValue() ).getValue();
    const double b = static_cast<const RealPos&>( base->getValue() ).getValue();
    value.setValue( log10(a) / log10(b) );
    return value;
}


/** Get argument rules */
const ArgumentRules& Func_log::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if (!rulesSet) 
    {
        argumentRules.push_back( new ValueRule( "x",    RealPos::getClassTypeSpec() ) );
        argumentRules.push_back( new ValueRule( "base", RealPos::getClassTypeSpec(), new RealPos(10.0) ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_log::getClassName(void) { 
    
    static std::string rbClassName = "Logarithm function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_log::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_log::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_log::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = Real::getClassTypeSpec();
    return returnTypeSpec;
}


/** We catch here the setting of the argument variables to store our parameters. */
void Func_log::setArgumentVariable(std::string const &name, const RbVariablePtr& var) {
    
    if ( name == "x" ) {
        x = var;
    }
    else if ( name == "base" ) {
        base = var;
    }
    else {
        RbFunction::setArgumentVariable(name, var);
    }
}

