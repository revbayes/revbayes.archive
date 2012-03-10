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
#include "Workspace.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <vector>
#include <set>


/** Clone object */
Func_dppServe* Func_dppServe::clone( void ) const {
	
    return new Func_dppServe( *this );
}


/** Execute function */
const RbLanguageObject& Func_dppServe::executeFunction( void ) {
    
//	// Expecting the allocation vector to be like: [0,0,1,0,1,2,2,2,2,2,3,0,3,1]
//	std::vector<unsigned int> allocVec = static_cast<const VectorNatural&>( (*args)[0].getValue() ).getValue();
//
//	std::sort(allocVec.begin(), allocVec.end()); // this makes [0,0,0,0,1,1,1,2,2,2,2,2,3,3]
//
//	int numClusters = *allocVec.end() + 1; 
//	std::vector<double> paramValues( numClusters, 0.0 );
//	
//	StochasticNode* paramVar( dynamic_cast<StochasticNode*>( (*args)[1].getDagNode() ) );
//    const DistributionContinuous& baseDistribution = static_cast<const DistributionContinuous&>( paramVar->getDistribution() );
//	
//	// Question: should this function take a stochastic node as an argument, or a distribution? I think maybe it should be a distribution...
//	
//	for(int i=0; i<numClusters; i++){
//		// paramValues[i] = ...
//	}
//    
//    values.setValue( paramValues );
	return values;
}


/** Get argument rules */
const ArgumentRules& Func_dppServe::getArgumentRules( void ) const {
	
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
	
    if ( !rulesSet ) {
		
        argumentRules.push_back( new ValueRule( "allocation", RbVector<Natural>::getClassTypeSpec() ) );
        argumentRules.push_back( new ValueRule( "distribution ~ G_0", Distribution::getClassTypeSpec() ) );
        rulesSet = true;
    }
	
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_dppServe::getClassName(void) { 
    
    static std::string rbClassName = "DPP parameter vector function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_dppServe::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_dppServe::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_dppServe::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RbVoid_name;
    return returnTypeSpec;
}

