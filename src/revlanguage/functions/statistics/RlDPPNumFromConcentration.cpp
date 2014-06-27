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
#include "RlDPPNumFromConcentration.h"
#include "RlClade.h"
#include "RlTimeTree.h"
#include "RateMatrix.h"
#include "RealPos.h"
#include "Topology.h"
#include "TypedDagNode.h"
#include "Vector.h"
#include "RbStatisticsHelper.h"

using namespace RevLanguage;

/** default constructor */
DPPNumFromConcentration::DPPNumFromConcentration( void ) : Function( ) {
    
}


/** Clone object */
DPPNumFromConcentration* DPPNumFromConcentration::clone( void ) const {
    
    return new DPPNumFromConcentration( *this );
}


RevObject* DPPNumFromConcentration::execute() {
	
    double cp = static_cast<const RealPos &>( args[0].getVariable()->getRevObject() ).getValue();
    double ne = static_cast<const RealPos &>( args[1].getVariable()->getRevObject() ).getValue();
	
	double meanCP = RevBayesCore::RbStatistics::Helper::dppExpectNumTableFromConcParam(cp, ne);
    RevBayesCore::ConstantNode<double> *constNode = new RevBayesCore::ConstantNode<double>("", new double(meanCP));
	
	RealPos* value = new RealPos( constNode );
	return value;
	
}


/* Get argument rules */
const ArgumentRules& DPPNumFromConcentration::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "concentration", true, RealPos::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "num_elements", true, RealPos::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& DPPNumFromConcentration::getClassName(void) { 
    
    static std::string rbClassName = "Func_dppMeanFromConc";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& DPPNumFromConcentration::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get return type */
const TypeSpec& DPPNumFromConcentration::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RealPos::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& DPPNumFromConcentration::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
