//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "MinFunction.h"
#include "Func_min.h"
#include "ModelVector.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_min::Func_min( void ) : TypedFunction<Real>( )
{
    
}


/** Clone object */
Func_min* Func_min::clone( void ) const
{
    
    return new Func_min( *this );
}


RevBayesCore::TypedFunction<double>* Func_min::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* arg = static_cast<const ModelVector<Real> &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::MinFunction* f = new RevBayesCore::MinFunction( arg );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_min::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "x", ModelVector<Real>::getClassTypeSpec(), "A vector of values.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_min::getClassType(void)
{
    
    static std::string revType = "Func_min";
    
	return revType;
}


/* Get class type spec describing type of object */
const TypeSpec& Func_min::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_min::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "min";
    
    return f_name;
}


const TypeSpec& Func_min::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
