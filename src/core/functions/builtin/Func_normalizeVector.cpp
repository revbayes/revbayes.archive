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
#include "ConstArgumentRule.h"
#include "DAGNode.h"
#include "DeterministicNode.h"
#include "Ellipsis.h"
#include "Func_normalizeVector.h"
#include "Integer.h"
#include "Probability.h"
#include "RbException.h"
#include "RbMathVector.h"
#include "RbUtil.h"
#include "RbVector.h"
#include "RealPos.h"
#include "Simplex.h"
#include "StochasticNode.h"
#include "TypeSpec.h"

#include <cassert>
#include <cmath>


/** Clone object */
Func_normalizeVector* Func_normalizeVector::clone( void ) const {

    return new Func_normalizeVector( *this );
}


/** Execute function */
RbPtr<RbLanguageObject> Func_normalizeVector::executeFunction( void ) {

    // Get first element
    const RbVector& tempVec    = static_cast<const RbVector& >( args[0]->getVariable()->getValue() );
    double          desiredSum = static_cast<const RealPos&  >( args[1]->getVariable()->getValue() ).getValue();
    
    // type conversion
    std::vector<double> unnormalizedVector;
    for (size_t i = 0; i < tempVec.size(); ++i) {
        unnormalizedVector.push_back( static_cast<const Real &>( tempVec[i] ).getValue() );
    }
    
    // normalize the vector
    RbMath::normalize(unnormalizedVector, desiredSum);
    RbVector *normalizedVector = new RbVector( Probability::getClassTypeSpec() );
    
    for (size_t i = 0; i < unnormalizedVector.size(); ++i) {
        normalizedVector->push_back( new Probability(unnormalizedVector[i]) );
    }
    
    
    return RbPtr<RbLanguageObject>( normalizedVector );
}


/** Get argument rules */
const ArgumentRules& Func_normalizeVector::getArgumentRules( void ) const {

    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;

    if (!rulesSet) {
        argumentRules.push_back( new ConstArgumentRule( "vector", TypeSpec( RbVector::getClassTypeSpec(), new TypeSpec(RealPos::getClassTypeSpec() ) ) ) );
        argumentRules.push_back( new ConstArgumentRule( "total",  new RealPos( 1.0 ) ) );
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
    
    static TypeSpec returnTypeSpec = TypeSpec( RbVector::getClassTypeSpec(), new TypeSpec( Probability::getClassTypeSpec() ) );
    
    return returnTypeSpec;
}

