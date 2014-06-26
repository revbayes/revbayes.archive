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
#include "RlDPPConcFromPriorMean.h"
#include "RlClade.h"
#include "RlTmrcaStatistic.h"
#include "RlTimeTree.h"
#include "RateMatrix.h"
#include "RealPos.h"
#include "Topology.h"
#include "TypedDagNode.h"
#include "Vector.h"
#include "RbStatisticsHelper.h"

using namespace RevLanguage;

/** default constructor */
DPPConcFromPriorMean::DPPConcFromPriorMean( void ) : Function( ) {
    
}


/** Clone object */
DPPConcFromPriorMean* DPPConcFromPriorMean::clone( void ) const {
    
    return new DPPConcFromPriorMean( *this );
}


RbLanguageObject* DPPConcFromPriorMean::execute() {

    double nc = static_cast<const RealPos &>( args[0].getVariable()->getValue() ).getValue();
    double ne = static_cast<const RealPos &>( args[1].getVariable()->getValue() ).getValue();
	
	double meanCP = RevBayesCore::RbStatistics::Helper::dppConcParamFromNumTables(nc, ne);
    RevBayesCore::ConstantNode<double> *constNode = new RevBayesCore::ConstantNode<double>("", new double(meanCP));
	
	RealPos* value = new RealPos( constNode );
	return value;

}


/* Get argument rules */
const ArgumentRules& DPPConcFromPriorMean::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "num_cats", true, RealPos::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "num_elements", true, RealPos::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& DPPConcFromPriorMean::getClassName(void) { 
    
    static std::string rbClassName = "DPP-approx_CP";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& DPPConcFromPriorMean::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get return type */
const TypeSpec& DPPConcFromPriorMean::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RealPos::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& DPPConcFromPriorMean::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
