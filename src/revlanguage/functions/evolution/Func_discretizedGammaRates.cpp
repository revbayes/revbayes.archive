//
//  Func_discretizedGammaRates.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 9/8/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "ArgumentRule.h"
#include "Ellipsis.h"
#include "Func_discretizedGammaRates.h"
#include "NormalizeVectorFunction.h"
#include "RbUtil.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"
#include "QuantileFunction.h"
#include "TypeSpec.h"
#include "Vector.h"
#include "GammaDistribution.h"
#include "VectorFunction.h"


using namespace RevLanguage;

Func_discretizedGammaRates::Func_discretizedGammaRates() : Function() {
    
}

/* Clone object */
Func_discretizedGammaRates* Func_discretizedGammaRates::clone( void ) const {
    
    return new Func_discretizedGammaRates( *this );
}


/** Execute function: We rely on getValue and overloaded push_back to provide functionality */
RevObject* Func_discretizedGammaRates::execute( void ) {
    
    const RevBayesCore::TypedDagNode< double > *shape = static_cast< RealPos & >( args[0].getVariable()->getRevObject() ).getDagNode();
    int nc = static_cast<const Integer &>( args[1].getVariable()->getRevObject() ).getValue();
    
    double binWidth = 1.0 / ((double)nc);
    double binMid = binWidth * 0.5;
    std::vector<const RevBayesCore::TypedDagNode<double> *> gRates = std::vector<const RevBayesCore::TypedDagNode<double>* >();
    for(int i=1; i<=nc; i++){
        RevBayesCore::ConstantNode<double> *quant = new RevBayesCore::ConstantNode<double>("", new double((double)i * binWidth - binMid));
        RevBayesCore::DeterministicNode<double> *tmp = new RevBayesCore::DeterministicNode<double>("", new RevBayesCore::QuantileFunction(quant,  new RevBayesCore::GammaDistribution(shape, shape)));
		gRates.push_back( tmp );
    }
    RevBayesCore::DeterministicNode<std::vector<double> > *discRates = new RevBayesCore::DeterministicNode<std::vector<double> >( "", new RevBayesCore::VectorFunction<double>(gRates) );
    RevBayesCore::NormalizeVectorFunction *func = new RevBayesCore::NormalizeVectorFunction( discRates );
    
    DeterministicNode<std::vector<double> > *detNode = new DeterministicNode<std::vector<double> >("", func, this->clone());
    
    Vector<RealPos> *theNormalizedVector = new Vector<RealPos>( detNode );
    
    return theNormalizedVector;
}


/** Get argument rules */
const ArgumentRules& Func_discretizedGammaRates::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "shape", true, RealPos::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "numCategories", true, Integer::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_discretizedGammaRates::getClassName(void) { 
    
    static std::string rbClassName = "Func_discretizedGammaRates";
    
	return rbClassName; 
}


/** Get class type spec describing type of object */
const RevLanguage::TypeSpec& Func_discretizedGammaRates::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get type spec */
const TypeSpec& Func_discretizedGammaRates::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_discretizedGammaRates::getReturnType( void ) const {
    
    return Vector<RealPos>::getClassTypeSpec();
}
