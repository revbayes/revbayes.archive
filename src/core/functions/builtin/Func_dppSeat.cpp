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

// Definition of the static type spec member
const TypeSpec Func_dppSeat::typeSpec(Func_dppSeat_name);
const TypeSpec Func_dppSeat::returnTypeSpec(RbVoid_name);

/** Clone object */
Func_dppSeat* Func_dppSeat::clone( void ) const {
	
    return new Func_dppSeat( *this );
}


/** Execute function */
RbPtr<RbLanguageObject> Func_dppSeat::executeFunction( void ) {
    
	double concentration = static_cast<const RealPos*>( (RbObject*)(*args)[0]->getValue() )->getValue();
	unsigned int num = static_cast<const Natural*>( (RbObject*)(*args)[1]->getValue() )->getValue();
	std::vector<int> tables;
	std::vector<int> clusters( num, 0 );
	
    RbPtr<RandomNumberGenerator> rng = GLOBAL_RNG;
	for(int i=0; i<(size_t)num; i++){
		if(i==0){
			tables.push_back( 1 );
			clusters[i] = 0;
		}
		else{
			double prNewTable = concentration / (i + concentration);
			double w = rng->uniform01();
			if(w < prNewTable){
				tables.push_back( 1 );
				clusters[i] = (int)tables.size()-1;
			}
			else{
				double u = rng->uniform01();
				double sum = 0.0;
				int j = 0;
				while(u > sum && j <= tables.size()){
					sum += (double)tables[j] / ((double)i);
					j++;
				}
				tables[j] += 1;
				clusters[i] = j-1;
			}
		}
	}
	return RbPtr<RbLanguageObject>( new VectorNatural( clusters ) );
}


/** Get argument rules */
RbPtr<const ArgumentRules> Func_dppSeat::getArgumentRules( void ) const {
	
    static RbPtr<ArgumentRules> argumentRules( new ArgumentRules() );
    static bool          rulesSet = false;
	
    if ( !rulesSet ) {
		
        argumentRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "concentration", RbObject_name ) ) );
        argumentRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "number", Natural_name ) ) );
        rulesSet = true;
    }
	
    return RbPtr<const ArgumentRules>( argumentRules );
}


/** Get class vector describing type of object */
const VectorString& Func_dppSeat::getClass( void ) const {
	
    static VectorString rbClass = VectorString( Func_dppSeat_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec& Func_dppSeat::getReturnType( void ) const {
	
    return returnTypeSpec;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Func_dppSeat::getTypeSpec(void) const {
    return typeSpec;
}

