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
#include "Real.h"
#include "RevObject.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

/** Constructor */
DistributionFunctionQuantile::DistributionFunctionQuantile( ContinuousDistribution *d ) : Function(), templateObject( d ), templateObjectPositive( NULL ) {
    
    argRules.push_back( new ArgumentRule("p", Probability::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
    const ArgumentRules &memberRules = templateObject->getParameterRules();
    for (std::vector<ArgumentRule*>::const_iterator it = memberRules.begin(); it != memberRules.end(); ++it) {
        argRules.push_back( (*it)->clone() );
    }
}

/** Constructor */
DistributionFunctionQuantile::DistributionFunctionQuantile( PositiveContinuousDistribution *d ) : Function(), templateObject( NULL ), templateObjectPositive( d ) {
    
    argRules.push_back( new ArgumentRule("p", Probability::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
    const ArgumentRules &memberRules = templateObjectPositive->getParameterRules();
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



DistributionFunctionQuantile::~DistributionFunctionQuantile( void )
{
    delete templateObject;
    delete templateObjectPositive;
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
RevPtr<Variable> DistributionFunctionQuantile::execute( void )
{
    
    Real* value = NULL;
    RevBayesCore::TypedDagNode<double>* arg = static_cast<const Probability &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    
    if ( templateObject != NULL ) {
        ContinuousDistribution* copyObject = templateObject->clone();
    
        for ( size_t i = 1; i < args.size(); i++ )
        {
        
            if ( args[i].isConstant() )
            {
                copyObject->setConstParameter( args[i].getLabel(), RevPtr<const Variable>( (Variable*) args[i].getVariable() ) );
            }
            else
            {
                copyObject->setParameter( args[i].getLabel(), args[i].getReferenceVariable() );
            }
        }
        
        RevBayesCore::ContinuousDistribution *d = copyObject->createDistribution();
        RevBayesCore::QuantileFunction* f = new RevBayesCore::QuantileFunction( arg, d );
        RevBayesCore::DeterministicNode<double> *detNode = new RevBayesCore::DeterministicNode<double>("", f);
        value = new Real( detNode );
    }
    else 
    {
        PositiveContinuousDistribution* copyObject = templateObjectPositive->clone();
        
        for ( size_t i = 1; i < args.size(); i++ )
        {
            
            if ( args[i].isConstant() )
            {
                copyObject->setConstParameter( args[i].getLabel(), RevPtr<const Variable>( (Variable*) args[i].getVariable() ) );
            }
            else
            {
                copyObject->setParameter( args[i].getLabel(), args[i].getReferenceVariable() );
            }
            
        }
        
        RevBayesCore::ContinuousDistribution *d = copyObject->createDistribution();
        RevBayesCore::QuantileFunction* f = new RevBayesCore::QuantileFunction( arg, d );
        RevBayesCore::DeterministicNode<double> *detNode = new RevBayesCore::DeterministicNode<double>("", f);
        value = new RealPos( detNode );
        
    }
    
    // return the value
    return new Variable( value );
}


/** Get argument rules */
const ArgumentRules& DistributionFunctionQuantile::getArgumentRules(void) const {
    
    return argRules;
}


/** Get Rev type of object */
const std::string& DistributionFunctionQuantile::getClassType(void) { 
    
    static std::string revType = "DistributionFunctionQuantile";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& DistributionFunctionQuantile::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}

/** Get type spec */
const TypeSpec& DistributionFunctionQuantile::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& DistributionFunctionQuantile::getReturnType(void) const {
    
    if ( templateObject != NULL ) 
    {
        return Real::getClassTypeSpec();
    }
    else if ( templateObjectPositive != NULL ) 
    {
        return RealPos::getClassTypeSpec();
    }
    else 
    {
        return Real::getClassTypeSpec();
    }
}
