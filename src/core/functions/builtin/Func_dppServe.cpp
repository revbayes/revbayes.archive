/**
 * @file
 * This file contains the declaration of Func_dppServe, which 
 * creates a vector of parameter classes and draws the value for each
 * from the base distribution G_0
 *
 * @brief Implementation of Func_dppServe
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

#include "DAGNode.h"
#include "DeterministicNode.h"
#include "Distribution.h"
#include "DistributionContinuous.h"
#include "Func_dppServe.h"
#include "Integer.h"
#include "Natural.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RealPos.h"
#include "RbConstants.h"
#include "RbStatisticsHelper.h"
#include "RandomNumberGenerator.h"
#include "RandomNumberFactory.h"
#include "StochasticNode.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "VectorInteger.h"
#include "VectorNatural.h"
#include "VectorString.h"
#include "VectorReal.h"
#include "Workspace.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <vector>
#include <set>

// Definition of the static type spec member
const TypeSpec Func_dppServe::typeSpec(Func_dppServe_name);
const TypeSpec Func_dppServe::returnTypeSpec(RbVoid_name);

/** Clone object */
Func_dppServe* Func_dppServe::clone( void ) const {
	
    return new Func_dppServe( *this );
}


/** Execute function */
RbLanguageObject* Func_dppServe::execute( void ) {
    
	// Expecting the allocation vector to be like: [0,0,1,0,1,2,2,2,2,2,3,0,3,1]
	std::vector<unsigned int> allocVec = static_cast<const VectorNatural*>( args[0].getValue() )->getValue();

	std::sort(allocVec.begin(), allocVec.end()); // this makes [0,0,0,0,1,1,1,2,2,2,2,2,3,3]

	int numClusters = *allocVec.end() + 1; 
	std::vector<double> paramValues( numClusters, 0.0 );
	
	StochasticNode* paramVar = dynamic_cast<StochasticNode*>( args[1].getDagNodePtr() );
	const DistributionContinuous* baseDistribution = static_cast<const DistributionContinuous*>( paramVar->getDistribution() );
	
	// Question: should this function take a stochastic node as an argument, or a distribution? I think maybe it should be a distribution...
	
	for(int i=0; i<numClusters; i++){
		// paramValues[i] = ...
	}
    
	return ( new VectorReal( paramValues ) );
}


/** Get argument rules */
const ArgumentRules& Func_dppServe::getArgumentRules( void ) const {
	
    static ArgumentRules argumentRules;
    static bool          rulesSet = false;
	
    if ( !rulesSet ) {
		
        argumentRules.push_back( new ValueRule( "allocation", VectorNatural_name ) );
        argumentRules.push_back( new ValueRule( "stochastic node ~ G_0", StochasticNode_name ) );
        rulesSet = true;
    }
	
    return argumentRules;
}


/** Get class vector describing type of object */
const VectorString& Func_dppServe::getClass( void ) const {
	
    static VectorString rbClass = VectorString( Func_dppServe_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec& Func_dppServe::getReturnType( void ) const {
	
    return returnTypeSpec;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Func_dppServe::getTypeSpec(void) const {
    return typeSpec;
}

