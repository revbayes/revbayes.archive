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

#include "ConstArgumentRule.h"
#include "Func_power.h"
#include "RealPos.h"
#include "Real.h"
#include "RbException.h"
#include "RbUtil.h"
#include "StochasticNode.h"
#include "TypeSpec.h"

#include <cassert>
#include <cmath>


/** Clone object */
Func_power* Func_power::clone( void ) const {
    
    return new Func_power( *this );
}


/** Execute function */
RbPtr<RbLanguageObject> Func_power::executeFunction(const std::vector<const RbObject *> &args) {
    
    const double a = static_cast<const Real *>( args[0] )->getValue();
    const double b = static_cast<const Real *>( args[1] )->getValue();

    return RbPtr<RbLanguageObject>( new Real( pow(a,b) ) );
}


/** Get argument rules */
const ArgumentRules& Func_power::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if (!rulesSet) 
    {
        argumentRules.push_back( new ConstArgumentRule( "base",     Real::getClassTypeSpec() ) );
        argumentRules.push_back( new ConstArgumentRule( "exponent", Real::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_power::getClassName(void) { 
    
    static std::string rbClassName = "Power function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_power::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_power::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_power::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = Real::getClassTypeSpec();
    return returnTypeSpec;
}

