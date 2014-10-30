//
//  Func_seed.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 9/8/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Func_seed.h"
#include "ArgumentRule.h"
#include "DeterministicNode.h"
#include "Ellipsis.h"
#include "Natural.h"
#include "RandomNumberGenerator.h"
#include "RandomNumberFactory.h"
#include "RbUtil.h"
#include "RevNullObject.h"
#include "RealPos.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"

using namespace RevLanguage;

Func_seed::Func_seed() : Procedure()
{
    
}

/* Clone object */
Func_seed* Func_seed::clone( void ) const
{
    
    return new Func_seed( *this );
}


/** Execute function: We rely on getValue and overloaded push_back to provide functionality */
RevPtr<Variable> Func_seed::execute( void )
{
    
    std::vector<unsigned int> s;
    const Natural &val1 = static_cast<const Natural &>( args[0].getVariable()->getRevObject() );
    s.push_back( (unsigned int) val1.getValue() );
    const Natural &val2 = static_cast<const Natural &>( args[1].getVariable()->getRevObject() );
    s.push_back( (unsigned int) val2.getValue() );
    
    RevBayesCore::RandomNumberGenerator *rng = RevBayesCore::GLOBAL_RNG;
    rng->setSeed( s );
        
    return NULL;
}


/** Get argument rules */
const ArgumentRules& Func_seed::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "seed1", Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        argumentRules.push_back( new ArgumentRule( "seed2", Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_seed::getClassType(void) { 
    
    static std::string revType = "Func_seed";
    
	return revType; 
}


/** Get class type spec describing type of object */
const TypeSpec& Func_seed::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/** Get type spec */
const TypeSpec& Func_seed::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_seed::getReturnType( void ) const {
    
    return RevNullObject::getClassTypeSpec();
}
