/**
 * @file
 * This file contains the implementation of Func_dppConFromExpNumClusters, which is
 * the function used to calculate the approximate, expected value of alpha from a given number
 * of clusters (tables).
 *
 * @brief Implementation of Func_unique
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
#include "DAGNode.h"
#include "DeterministicNode.h"
#include "Func_dppConFromExpNumClusters.h"
#include "Integer.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RealPos.h"
#include "RbStatisticsHelper.h"
#include "StochasticNode.h"
#include "TypeSpec.h"

#include <cassert>
#include <cmath>


/** Clone object */
Func_dppConFromExpNumClusters* Func_dppConFromExpNumClusters::clone( void ) const {
	
    return new Func_dppConFromExpNumClusters( *this );
}


/** Execute function */
RbPtr<RbLanguageObject> Func_dppConFromExpNumClusters::executeFunction( void ) {
    
	double clust  = static_cast<const RealPos&>( *args[0]->getVariable()->getValue() ).getValue();
	double num    = static_cast<const RealPos&>( *args[1]->getVariable()->getValue() ).getValue();
		
	double p = RbStatistics::Helper::dppConcParamFromNumTables(clust, num);

	return RbPtr<RbLanguageObject>( new Real( p ) );
}


/** Get argument rules */
const ArgumentRules& Func_dppConFromExpNumClusters::getArgumentRules( void ) const {
	
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
	
    if ( !rulesSet ) {
		
        argumentRules.push_back( new ConstArgumentRule( "clusters", RealPos::getClassTypeSpec() ) );
        argumentRules.push_back( new ConstArgumentRule( "number",   RealPos::getClassTypeSpec() ) );
        rulesSet = true;
    }
	
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_dppConFromExpNumClusters::getClassName(void) { 
    
    static std::string rbClassName = "DPP concentration parameter calculation function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_dppConFromExpNumClusters::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_dppConFromExpNumClusters::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_dppConFromExpNumClusters::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = Real::getClassTypeSpec();
    return returnTypeSpec;
}

