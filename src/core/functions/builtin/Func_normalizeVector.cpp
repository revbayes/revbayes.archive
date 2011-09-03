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

// Definition of the static type spec member
const TypeSpec Func_normalizeVector::typeSpec(Func_normalizeVector_name);

/** Clone object */
Func_normalizeVector* Func_normalizeVector::clone( void ) const {

    return new Func_normalizeVector( *this );
}


/** Execute function */
RbLanguageObject* Func_normalizeVector::execute( void ) {

    // Get first element
    std::vector<double> tempVec    = static_cast<const VectorRealPos*>( args[0].getValue() )->getValue();
    double              desiredSum = static_cast<const RealPos*      >( args[1].getValue() )->getValue();
    
    // normalize the vector
    RbMath::normalize(tempVec, desiredSum);
        
    return new Simplex( tempVec );
}


/** Get argument rules */
const ArgumentRules& Func_normalizeVector::getArgumentRules( void ) const {

    static ArgumentRules argumentRules;
    static bool          rulesSet = false;

    if (!rulesSet)
		{
        argumentRules.push_back( new ValueRule( "", VectorRealPos_name ) );
        argumentRules.push_back( new ValueRule( "", new RealPos( 1.0 ) ) );
        rulesSet = true;
		}

    return argumentRules;
}


/** Get class vector describing type of object */
const VectorString& Func_normalizeVector::getClass( void ) const {

    static VectorString rbClass = VectorString( Func_normalizeVector_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec Func_normalizeVector::getReturnType( void ) const {

    return TypeSpec( Simplex_name );
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Func_normalizeVector::getTypeSpec(void) const {
    return typeSpec;
}

