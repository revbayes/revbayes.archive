/**
 * @file
 * This file contains the implementation of Func_range, which is
 * the function used to compute range (colon) vectors.
 *
 * @brief Implementation of Func_range
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-05-04 18:03:37 +0200 (Fri, 04 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @interface RbFunction
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id: Func_source.cpp 1485 2012-05-04 16:03:37Z hoehna $
 */

#include "Argument.h"
#include "ArgumentRule.h"
#include "Func_range.h"
#include "Integer.h"
#include "ModelVector.h"
#include "RbException.h"
#include "TypeSpec.h"

using namespace RevLanguage;

/** Default constructor */
Func_range::Func_range( void ) : Procedure() {
    
}


/** Clone object */
Func_range* Func_range::clone( void ) const {
    
    return new Func_range( *this );
}


/** Execute function */
RevPtr<Variable> Func_range::execute( void )
{
    
    int f = static_cast<const Integer &>( args[0].getVariable()->getRevObject() ).getValue();
    int l = static_cast<const Integer &>( args[1].getVariable()->getRevObject() ).getValue();
    
    ModelVector<Integer> *range = new ModelVector<Integer>( RevBayesCore::RbVector<int>() );
    if (f < l)
    {
        for ( int i = f; i <= l; i++ )
        {
            range->push_back( Integer(i) );
        }
    }
    else
    {
        for ( int i = f; i >= l; i-- )
        {
            range->push_back( Integer(i) );
        }
    }
    
    return new Variable( range );
}


/** Get argument rules */
const ArgumentRules& Func_range::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if (!rulesSet)
    {
        
        argumentRules.push_back( new ArgumentRule( "first", Integer::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        argumentRules.push_back( new ArgumentRule( "last" , Integer::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_range::getClassType(void) { 
    
    static std::string revType = "Func_range";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_range::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}

/** Get type spec */
const TypeSpec& Func_range::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_range::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = ModelVector<Integer>::getClassTypeSpec();
    
    return returnTypeSpec;
}

