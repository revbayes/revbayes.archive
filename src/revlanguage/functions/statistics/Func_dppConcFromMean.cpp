//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Clade.h"
#include "ConstantNode.h"
#include "DeterministicNode.h"
#include "Func_dppConcFromMean.h"
#include "RlClade.h"
#include "RlTimeTree.h"
#include "RateMatrix.h"
#include "RbStatisticsHelper.h"
#include "RealPos.h"
#include "Topology.h"
#include "TypedDagNode.h"
#include "Vector.h"

using namespace RevLanguage;

/** default constructor */
Func_dppConcFromMean::Func_dppConcFromMean( void ) : Function( ) {
    
}


/** Clone object */
Func_dppConcFromMean* Func_dppConcFromMean::clone( void ) const {
    
    return new Func_dppConcFromMean( *this );
}


RevPtr<Variable> Func_dppConcFromMean::execute() {

    double nc = static_cast<const RealPos &>( args[0].getVariable()->getRevObject() ).getValue();
    double ne = static_cast<const RealPos &>( args[1].getVariable()->getRevObject() ).getValue();
	
	double meanCP = RevBayesCore::RbStatistics::Helper::dppConcParamFromNumTables(nc, ne);
    RevBayesCore::ConstantNode<double> *constNode = new RevBayesCore::ConstantNode<double>("", new double(meanCP));
	
	RealPos* value = new RealPos( constNode );

	return new Variable( value );
}


/* Get argument rules */
const ArgumentRules& Func_dppConcFromMean::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "num_cats", true, RealPos::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "num_elements", true, RealPos::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_dppConcFromMean::getClassName(void) { 
    
    static std::string rbClassName = "Func_dppConcFromMean";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_dppConcFromMean::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get return type */
const TypeSpec& Func_dppConcFromMean::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RealPos::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_dppConcFromMean::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
