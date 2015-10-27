//
//  Func_lnProbability.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 3/12/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#include "Func_lnProbability.h"
#include "Probability.h"
#include "Real.h"
#include "LnProbabilityFunction.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_lnProbability::Func_lnProbability( void ) : TypedFunction<Real>( )
{
    
}


/** Clone object */
Func_lnProbability* Func_lnProbability::clone( void ) const
{
    
    return new Func_lnProbability( *this );
}


RevBayesCore::TypedFunction<double>* Func_lnProbability::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<double>* x = static_cast<const Real&>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode<double>* y = static_cast<RevBayesCore::StochasticNode<double>* >(x);
    RevBayesCore::LnProbabilityFunction* f = new RevBayesCore::LnProbabilityFunction( y );
    
    return f;
}

/* Get argument rules */
const ArgumentRules& Func_lnProbability::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "x", Real::getClassTypeSpec(), "The value.", ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_lnProbability::getClassType(void)
{
    
    static std::string revType = "Func_lnProbability";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_lnProbability::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


const TypeSpec& Func_lnProbability::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
