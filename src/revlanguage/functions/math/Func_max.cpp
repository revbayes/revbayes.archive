//
//  Func_max.cpp
//  RevBayesCore
//
//  Created by Bastien Boussau on 02/11/14.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "MaxFunction.h"
#include "Func_max.h"
#include "ModelVector.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_max::Func_max( void ) : TypedFunction<Real>( ) {
    
}


/** Clone object */
Func_max* Func_max::clone( void ) const
{
    
    return new Func_max( *this );
}


RevBayesCore::TypedFunction<double>* Func_max::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* arg = static_cast<const ModelVector<Real> &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::MaxFunction* f = new RevBayesCore::MaxFunction( arg );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_max::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "x", ModelVector<Real>::getClassTypeSpec(), "A vector of numbers.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_max::getClassType(void)
{
    
    static std::string revType = "Func_max";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_max::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/**
 * Get the primary Rev name for this function.
 */
const std::string& Func_max::getFunctionName( void ) const
{
    // create a static name variable that is the same for all instance of this class
    static std::string f_name = "max";
    
    return f_name;
}


const TypeSpec& Func_max::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
