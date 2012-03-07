/**
 * @file
 * This file contains the implementation of Func_mean.
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
#include "Ellipsis.h"
#include "Func_mean.h"
#include "RealPos.h"
#include "Real.h"
#include "RbException.h"
#include "RbUtil.h"
#include "StochasticNode.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "Vector.h"

#include <cassert>
#include <cmath>


Func_mean::Func_mean(void) : RbFunction() {
    
}


/** Clone object */
Func_mean* Func_mean::clone( void ) const {
    
    return new Func_mean( *this );
}


/** Execute function */
const RbLanguageObject& Func_mean::executeFunction( void ) {
    
    double m = 0.0;
    const Vector& v = static_cast<const Vector&>( x->getValue() );
    for (size_t i = 0; i < v.size(); i++) {
        m += static_cast<const Real&>( v.getElement(i) ).getValue();
    }
    
    m /= v.size();
    
//    for (std::vector<RbVariablePtr>::iterator i = x.begin(); i != x.end(); i++) {
//        const Vector& v = static_cast<const Vector&>( (*i)->getValue() );
//        for (size_t i = 0; i < v.size(); i++) {
//            m += static_cast<const Real&>( v.getElement(i) ).getValue();
//            count++;
//        }
//    }
    
    value.setValue( m );
    return value;
}


/** Get argument rules */
const ArgumentRules& Func_mean::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if (!rulesSet) 
    {
        argumentRules.push_back( new ValueRule( "x", TypeSpec(Vector::getClassTypeSpec(), new TypeSpec( Real::getClassTypeSpec() ) ) ) );
//        argumentRules.push_back( new Ellipsis( TypeSpec(Vector::getClassTypeSpec(), new TypeSpec( Real::getClassTypeSpec() ) ) ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_mean::getClassName(void) { 
    
    static std::string rbClassName = "meanonential function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_mean::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_mean::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_mean::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RealPos::getClassTypeSpec();
    return returnTypeSpec;
}


/** We catch here the setting of the argument variables to store our parameters. */
void Func_mean::setArgumentVariable(std::string const &name, const Variable* var) {
    
    if ( name == "x" ) {
        x = var;
    }
    else {
        RbFunction::setArgumentVariable(name, var);
    }
}

