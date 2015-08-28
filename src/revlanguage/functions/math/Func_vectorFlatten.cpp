//
//  Func_vectorFlatten.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 4/2/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//


#include "ArgumentRule.h"
#include "Func_vectorFlatten.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "RlSimplex.h"
#include "VectorFlattenFunction.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;


/** Default constructor */
Func_vectorFlatten::Func_vectorFlatten( void ) :
TypedFunction<ModelVector<RealPos> >()
{
}


/** Clone object */
Func_vectorFlatten* Func_vectorFlatten::clone( void ) const
{
    return new Func_vectorFlatten( *this );
}


/** Execute function: Compute simplex from vector of RealPos values */
RevBayesCore::TypedFunction< RevBayesCore::RbVector<double> >* Func_vectorFlatten::createFunction( void ) const
{
    
    const RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RbVector<double> > >* vec;
    vec = static_cast< const ModelVector<ModelVector<Real> >& >( args[0].getVariable()->getRevObject() ).getDagNode();
    
    RevBayesCore::VectorFlattenFunction* func = new RevBayesCore::VectorFlattenFunction( vec );
    
    return func;
}


/** Get argument rules */
const ArgumentRules& Func_vectorFlatten::getArgumentRules( void ) const
{
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        argumentRules.push_back( new ArgumentRule( "x", ModelVector<ModelVector<Real> >::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object (static version) */
const std::string& Func_vectorFlatten::getClassType( void )
{
    static std::string revType = "Func_vectorFlatten";
    
	return revType;
}


/** Get Rev type spec of object (static version) */
const TypeSpec& Func_vectorFlatten::getClassTypeSpec( void )
{
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), &Function::getClassTypeSpec() );
    
	return revTypeSpec;
}


/** Get Rev type spec from an instance of the object */
const TypeSpec& Func_vectorFlatten::getTypeSpec( void ) const
{
    return getClassTypeSpec();
}

