#include "Argument.h"
#include "ArgumentRule.h"
#include "Func_type.h"
#include "RbException.h"
#include "RlString.h"
#include "RlUtils.h"
#include "TypeSpec.h"

#include <fstream>

using namespace RevLanguage;

/** Default constructor */
Func_type::Func_type( void ) : Procedure() {
    
}


/** Clone object */
Func_type* Func_type::clone( void ) const {
    
    return new Func_type( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_type::execute( void ) {
    
    RlString* type = new RlString( args[0].getVariable()->getRevObject().getType() );
    
    return new RevVariable( type );
}


/** Get argument rules */
const ArgumentRules& Func_type::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "x", RevObject::getClassTypeSpec(), "A variable.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        rulesSet = true;
        
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_type::getClassType(void)
{
    
    static std::string revType = "Func_type";
    
	return revType; 
}


/** Get class type spec describing type of object */
const TypeSpec& Func_type::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/**
 * Get the primary Rev name for this function.
 */
const std::string& Func_type::getFunctionName( void ) const
{
    // create a static name variable that is the same for all instance of this class
    static std::string f_name = "type";
    
    return f_name;
}


/** Get type spec */
const TypeSpec& Func_type::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_type::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = RlString::getClassTypeSpec();
    
    return returnTypeSpec;
}

