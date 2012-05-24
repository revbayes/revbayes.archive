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
#include "ConstArgumentRule.h"
#include "DAGNode.h"
#include "DeterministicNode.h"
#include "Ellipsis.h"
#include "Func_mean.h"
#include "RealPos.h"
#include "Real.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RlVector.h"
#include "StochasticNode.h"
#include "TypeSpec.h"

#include <cassert>
#include <cmath>


Func_mean::Func_mean(void) : RbFunction() {
    
}


/** Clone object */
Func_mean* Func_mean::clone( void ) const {
    
    return new Func_mean( *this );
}


/** Execute function */
RbPtr<RbLanguageObject> Func_mean::executeFunction(const std::vector<const RbObject *> &args) {
    
    double m = 0.0;
    const RlVector<Real>& v = static_cast<const RlVector<Real>&>( *args[0] );
    for (size_t i = 0; i < v.size(); i++) {
        m += v[i];
    }
    
    m /= v.size();
    
    return RbPtr<RbLanguageObject>( new Real(m) );
}


/** Get argument rules */
const ArgumentRules& Func_mean::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if (!rulesSet) 
    {
        argumentRules.push_back( new ConstArgumentRule( "x", TypeSpec( RlVector<Real>::getClassTypeSpec(), new TypeSpec(Real::getClassTypeSpec() ) ) ) );
//        argumentRules.push_back( new Ellipsis( TypeSpec(Vector::getClassTypeSpec(), new TypeSpec( Real::getClassTypeSpec() ) ) ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_mean::getClassName(void) { 
    
    static std::string rbClassName = "mean function";
    
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

