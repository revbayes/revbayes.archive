//
//  Func_epoch.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 3/17/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#include "Func_epoch.h"
#include "EpochRateMatrixFunction.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_epoch::Func_epoch( void ) : TypedFunction<RateGenerator>( ) {
    
}


/** Clone object */
Func_epoch* Func_epoch::clone( void ) const {
    
    return new Func_epoch( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::RateGenerator >* Func_epoch::createFunction( void ) const
{
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RateGenerator> >* rm = static_cast<const ModelVector<RateGenerator> &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* t = static_cast<const ModelVector<RealPos> &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* r = static_cast<const ModelVector<RealPos> &>( this->args[2].getVariable()->getRevObject() ).getDagNode();

    
    // sanity check
    if ( t->getValue().size() != rm->getValue().size() )
    {
        throw RbException( "The number of rate matrices and epochs do not match" );
    }
    
    RevBayesCore::EpochRateMatrixFunction* f = new RevBayesCore::EpochRateMatrixFunction( rm, t, r );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_epoch::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        argumentRules.push_back( new ArgumentRule( "Q"              , ModelVector<RateGenerator>::getClassTypeSpec(), "The per epoch rate matrices", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        argumentRules.push_back( new ArgumentRule( "times"          , ModelVector<RealPos>::getClassTypeSpec(),       "The times of the epochs.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        argumentRules.push_back( new ArgumentRule( "rates"          , ModelVector<RealPos>::getClassTypeSpec(),       "The rate multipliers per epoch.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_epoch::getClassType(void) {
    
    static std::string revType = "Func_epoch";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_epoch::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


const TypeSpec& Func_epoch::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}