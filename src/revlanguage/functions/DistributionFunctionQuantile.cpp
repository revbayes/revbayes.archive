//
//  DistributionFunctionQuantile.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 3/8/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#include "DistributionFunctionQuantile.h"

#include "ArgumentRule.h"
#include "DeterministicNode.h"
#include "DistributionFunctionPdf.h"
#include "Probability.h"
#include "ProbabilityDensityFunction.h"
#include "QuantileFunction.h"
#include "RbLanguageObject.h"
#include "Real.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

/** Constructor */
DistributionFunctionQuantile::DistributionFunctionQuantile( ContinuousDistribution *d ) : Function(), templateObject( d ), templateObjectPositive( NULL ) {
    
    argRules.push_back( new ArgumentRule("p", true, Probability::getClassTypeSpec()));
    const ArgumentRules &memberRules = templateObject->getMemberRules();
    for (std::vector<ArgumentRule*>::const_iterator it = memberRules.begin(); it != memberRules.end(); ++it) {
        argRules.push_back( (*it)->clone() );
    }
}

/** Constructor */
DistributionFunctionQuantile::DistributionFunctionQuantile( PositiveContinuousDistribution *d ) : Function(), templateObject( NULL ), templateObjectPositive( d ) {
    
    argRules.push_back( new ArgumentRule("p", true, Probability::getClassTypeSpec()));
    const ArgumentRules &memberRules = templateObjectPositive->getMemberRules();
    for (std::vector<ArgumentRule*>::const_iterator it = memberRules.begin(); it != memberRules.end(); ++it) {
        argRules.push_back( (*it)->clone() );
    }
}


/** Constructor */
DistributionFunctionQuantile::DistributionFunctionQuantile(const DistributionFunctionQuantile& obj) : Function(obj), argRules( obj.argRules )  {
    
    if ( obj.templateObject != NULL ) {
        templateObject = obj.templateObject->clone();
    }
    else {
        templateObject = NULL;
    }
    
    if ( obj.templateObjectPositive != NULL ) {
        templateObjectPositive = obj.templateObjectPositive->clone();
    }
    else {
        templateObjectPositive = NULL;
    }
    
}


DistributionFunctionQuantile& DistributionFunctionQuantile::operator=(const DistributionFunctionQuantile &c) {
    
    if (this != &c) {
        Function::operator=(c);
        
        if ( c.templateObject != NULL ) {
            templateObject = c.templateObject->clone();
        }
        else {
            templateObject = NULL;
        }
        
        if ( c.templateObjectPositive != NULL ) {
            templateObjectPositive = c.templateObjectPositive->clone();
        }
        else {
            templateObjectPositive = NULL;
        }
        
        argRules = c.argRules;
    }
    
    return *this;
}


/** Clone the object */
DistributionFunctionQuantile* DistributionFunctionQuantile::clone(void) const {
    
    return new DistributionFunctionQuantile(*this);
}


/** Execute function: we reset our template object here and give out a copy */
RbLanguageObject* DistributionFunctionQuantile::execute( void ) {
    
    RevBayesCore::ContinuousDistribution *d = NULL;
    
    if ( templateObject != NULL ) {
        ContinuousDistribution* copyObject = templateObject->clone();
    
        for ( size_t i = 1; i < args.size(); i++ ) {
        
            if ( args[i].isConstant() ) {
                copyObject->setConstMember( args[i].getLabel(), RbPtr<const Variable>( (Variable*) args[i].getVariable() ) );
            } else {
                copyObject->setMember( args[i].getLabel(), args[i].getReferenceVariable() );
            }
        }
        
        d = copyObject->createDistribution();
    }
    else {
        PositiveContinuousDistribution* copyObject = templateObjectPositive->clone();
        
        for ( size_t i = 1; i < args.size(); i++ ) {
            
            if ( args[i].isConstant() ) {
                copyObject->setConstMember( args[i].getLabel(), RbPtr<const Variable>( (Variable*) args[i].getVariable() ) );
            } else {
                copyObject->setMember( args[i].getLabel(), args[i].getReferenceVariable() );
            }
        }
        
        d = copyObject->createDistribution();
        
    }
    
    
    RevBayesCore::TypedDagNode<double>* arg = static_cast<const Probability &>( this->args[0].getVariable()->getValue() ).getValueNode();
    RevBayesCore::QuantileFunction* f = new RevBayesCore::QuantileFunction( arg, d );
    RevBayesCore::DeterministicNode<double> *detNode = new RevBayesCore::DeterministicNode<double>("", f);
    
    Real* value = new Real( detNode );
    
    return value;
}


/** Get argument rules */
const ArgumentRules& DistributionFunctionQuantile::getArgumentRules(void) const {
    
    return argRules;
}


/** Get class name of object */
const std::string& DistributionFunctionQuantile::getClassName(void) { 
    
    static std::string rbClassName = "Quantile";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& DistributionFunctionQuantile::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& DistributionFunctionQuantile::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& DistributionFunctionQuantile::getReturnType(void) const {
    
    return Real::getClassTypeSpec();
}
