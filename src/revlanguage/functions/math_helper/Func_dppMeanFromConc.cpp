//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Clade.h"
#include "ConstantNode.h"
#include "DeterministicNode.h"
#include "Func_dppMeanFromConc.h"
#include "ModelVector.h"
#include "RbStatisticsHelper.h"
#include "RlClade.h"
#include "RlTimeTree.h"
#include "RealPos.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_dppMeanFromConc::Func_dppMeanFromConc( void ) : Procedure( )
{
    
}


/** Clone object */
Func_dppMeanFromConc* Func_dppMeanFromConc::clone( void ) const
{
    
    return new Func_dppMeanFromConc( *this );
}


RevPtr<RevVariable> Func_dppMeanFromConc::execute()
{
	
    double cp = static_cast<const RealPos &>( args[0].getVariable()->getRevObject() ).getValue();
    double ne = static_cast<const RealPos &>( args[1].getVariable()->getRevObject() ).getValue();
	
	double numCat = RevBayesCore::RbStatistics::Helper::dppExpectNumTableFromConcParam(cp, ne);
	
	RealPos* value = new RealPos( numCat );
	return new RevVariable( value );
}


/* Get argument rules */
const ArgumentRules& Func_dppMeanFromConc::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "concentration", RealPos::getClassTypeSpec(), "The concentration parameter of the DPP.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "numElements" , RealPos::getClassTypeSpec(), "The number of elements of the DPP.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_dppMeanFromConc::getClassType(void)
{
    
    static std::string revType = "Func_dppMeanFromConc";
    
	return revType; 
}


/* Get class type spec describing type of object */
const TypeSpec& Func_dppMeanFromConc::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/* Get return type */
const TypeSpec& Func_dppMeanFromConc::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = RealPos::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_dppMeanFromConc::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
