/**
 * @file
 * This file contains the declaration of Func_normalizeVector, which 
 * normalizes a vector of positive real numbers.
 *
 * @brief Implementation of Func_normalizeVector
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
#include "Func_normalizeVector.h"
#include "Integer.h"
#include "RbException.h"
#include "RbMathVector.h"
#include "RbUtil.h"
#include "RealPos.h"
#include "Simplex.h"
#include "StochasticNode.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "VectorReal.h"
#include "VectorRealPos.h"
#include "VectorString.h"

#include <cassert>
#include <cmath>


/** Clone object */
Func_normalizeVector* Func_normalizeVector::clone( void ) const {

    return new Func_normalizeVector( *this );
}


/** Execute function */
const RbLanguageObject& Func_normalizeVector::executeFunction( void ) {

    // Get first element
    std::vector<double> tempVec    = static_cast<const VectorRealPos&>( vector->getValue() ).getValue();
    double              desiredSum = static_cast<const RealPos&      >( total->getValue() ).getValue();
    
    // normalize the vector
    RbMath::normalize(tempVec, desiredSum);
    normalizedVector.setValue( tempVec );
    
    return normalizedVector;
}


/** Get argument rules */
const ArgumentRules& Func_normalizeVector::getArgumentRules( void ) const {

    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;

    if (!rulesSet)
		{
        argumentRules.push_back( new ValueRule( "vector", VectorRealPos::getClassTypeSpec() ) );
        argumentRules.push_back( new ValueRule( "total",  new RealPos( 1.0 ) ) );
        rulesSet = true;
		}

    return argumentRules;
}


/** Get class name of object */
const std::string& Func_normalizeVector::getClassName(void) { 
    
    static std::string rbClassName = "Normalize vector function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_normalizeVector::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_normalizeVector::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_normalizeVector::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = VectorRealPos::getClassTypeSpec();
    
    return returnTypeSpec;
}


/** We catch here the setting of the argument variables to store our parameters. */
void Func_normalizeVector::setArgumentVariable(std::string const &name, const Variable* var) {
    
    if ( name == "vector" ) {
        vector = var;
    }
    else if ( name == "total" ) {
        total = var;
    }
    else {
        RbFunction::setArgumentVariable(name, var);
    }
}

