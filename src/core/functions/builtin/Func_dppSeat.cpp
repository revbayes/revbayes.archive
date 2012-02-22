/**
 * @file
 * This file contains the declaration of Func_dppSeat, which 
 * seats data elements at "tables" and returns an allocation vector
 *
 * @brief Implementation of Func_dppSeat
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
#include "DistributionContinuous.h"
#include "Func_dppSeat.h"
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

#include <cassert>
#include <cmath>
#include <vector>
#include <set>



/** Clone object */
Func_dppSeat* Func_dppSeat::clone( void ) const {
	
    return new Func_dppSeat( *this );
}


/** Execute function */
const RbLanguageObject& Func_dppSeat::executeFunction( void ) {
    
//	double          concentration   = static_cast<const RealPos&>( (*args)[0].getValue() ).getValue();
//	unsigned int    num             = static_cast<const Natural&>( (*args)[1].getValue() ).getValue();
//	std::vector<int> tables;
//	std::vector<int> clusters( num, 0 );
//	
//    RandomNumberGenerator* rng = GLOBAL_RNG;
//	for(size_t i=0; i<(size_t)num; i++){
//		if(i==0){
//			tables.push_back( 1 );
//			clusters[i] = 0;
//		}
//		else{
//			double prNewTable = concentration / (i + concentration);
//			double w = rng->uniform01();
//			if(w < prNewTable){
//				tables.push_back( 1 );
//				clusters[i] = (int)tables.size()-1;
//			}
//			else{
//				double u = rng->uniform01();
//				double sum = 0.0;
//				size_t j = 0;
//				while(u > sum && j <= tables.size()){
//					sum += (double)tables[j] / ((double)i);
//					j++;
//				}
//				tables[j] += 1;
//				clusters[i] = (int)(j - 1);
//			}
//		}
//	}
//    allocation.setValue( clusters );
	return allocation;
}


/** Get argument rules */
const ArgumentRules& Func_dppSeat::getArgumentRules( void ) const {
	
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
	
    if ( !rulesSet ) {
		
        argumentRules.push_back( new ValueRule( "concentration", RbObject::getClassTypeSpec() ) );
        argumentRules.push_back( new ValueRule( "number",        Natural::getClassTypeSpec()  ) );
        rulesSet = true;
    }
	
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_dppSeat::getClassName(void) { 
    
    static std::string rbClassName = "DPP allocation vector function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_dppSeat::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_dppSeat::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_dppSeat::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RbVoid_name;
    return returnTypeSpec;
}

