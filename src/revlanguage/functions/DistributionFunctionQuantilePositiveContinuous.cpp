//
//  DistributionFunctionQuantilePositiveContinuous.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 3/8/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#include "DistributionFunctionQuantilePositiveContinuous.h"

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
DistributionFunctionQuantilePositiveContinuous::DistributionFunctionQuantilePositiveContinuous( PositiveContinuousDistribution *d ) : TypedFunction<RealPos>(),
templateObjectPositive( d )
{
    
    argRules.push_back( new ArgumentRule("p", Probability::getClassTypeSpec(), "The probability for this quantile.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    const ArgumentRules &memberRules = templateObjectPositive->getParameterRules();
    for (std::vector<ArgumentRule*>::const_iterator it = memberRules.begin(); it != memberRules.end(); ++it)
    {
        argRules.push_back( (*it)->clone() );
    }
    
}


/** Constructor */
DistributionFunctionQuantilePositiveContinuous::DistributionFunctionQuantilePositiveContinuous(const DistributionFunctionQuantilePositiveContinuous& obj) : TypedFunction<RealPos>(obj),
argRules( obj.argRules )
{
    
    if ( obj.templateObjectPositive != NULL )
    {
        templateObjectPositive = obj.templateObjectPositive->clone();
    }
    else
    {
        templateObjectPositive = NULL;
    }
    
}



DistributionFunctionQuantilePositiveContinuous::~DistributionFunctionQuantilePositiveContinuous( void )
{
    delete templateObjectPositive;
}

DistributionFunctionQuantilePositiveContinuous& DistributionFunctionQuantilePositiveContinuous::operator=(const DistributionFunctionQuantilePositiveContinuous &c) {
    
    if (this != &c) {
        Function::operator=(c);
        
        delete templateObjectPositive;
        
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
DistributionFunctionQuantilePositiveContinuous* DistributionFunctionQuantilePositiveContinuous::clone(void) const {
    
    return new DistributionFunctionQuantilePositiveContinuous(*this);
}


RevBayesCore::TypedFunction<double>* DistributionFunctionQuantilePositiveContinuous::createFunction( void ) const
{
    
    RevBayesCore::TypedFunction<double>* f = NULL;
    RevBayesCore::TypedDagNode<double>* arg = static_cast<const Probability &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    
    PositiveContinuousDistribution* copyObject = templateObjectPositive->clone();
    
    for ( size_t i = 1; i < args.size(); i++ )
    {
            
        if ( args[i].isConstant() )
        {
            copyObject->setConstParameter( args[i].getLabel(), RevPtr<const RevVariable>( (const RevVariable*) args[i].getVariable() ) );
        }
        else
        {
            copyObject->setParameter( args[i].getLabel(), args[i].getReferenceVariable() );
        }
        
    }
        
    RevBayesCore::ContinuousDistribution *d = copyObject->createDistribution();
    f = new RevBayesCore::QuantileFunction( arg, d );
    
    
    // return the value
    return f;
}


/** Get argument rules */
const ArgumentRules& DistributionFunctionQuantilePositiveContinuous::getArgumentRules(void) const {
    
    return argRules;
}


/** Get Rev type of object */
const std::string& DistributionFunctionQuantilePositiveContinuous::getClassType(void) {
    
    static std::string revType = "DistributionFunctionQuantilePositiveContinuous";
    
    return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& DistributionFunctionQuantilePositiveContinuous::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
}

/** Get type spec */
const TypeSpec& DistributionFunctionQuantilePositiveContinuous::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
