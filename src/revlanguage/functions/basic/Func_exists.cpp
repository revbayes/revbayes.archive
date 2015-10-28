#include "Argument.h"
#include "ArgumentRule.h"
#include "Func_exists.h"
#include "RbException.h"
#include "RlString.h"
#include "RlUtils.h"
#include "TypeSpec.h"
#include "Workspace.h"

using namespace RevLanguage;

/** Default constructor */
Func_exists::Func_exists( void ) : Procedure() {
    
}


/** Clone object */
Func_exists* Func_exists::clone( void ) const
{
    
    return new Func_exists( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_exists::execute( void )
{
    
    const std::string& name = static_cast<const RlString &>( args[0].getVariable()->getRevObject() ).getValue();
    
    bool exists = Workspace::userWorkspace().existsVariable( name );
    
    return new RevVariable( new RlBoolean( exists ) );
}


/** Get argument rules */
const ArgumentRules& Func_exists::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "name", RlString::getClassTypeSpec(), "The name of the variable we wish to check for existence.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_exists::getClassType(void)
{
    
    static std::string revType = "Func_exists";
    
    return revType;
}


/**
 * Get the primary Rev name for this function.
 */
const std::string& Func_exists::getFunctionName( void ) const
{
    // create a static name variable that is the same for all instance of this class
    static std::string f_name = "exists";
    
    return f_name;
}


/** Get class type spec describing type of object */
const TypeSpec& Func_exists::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/** Get type spec */
const TypeSpec& Func_exists::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_exists::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RlBoolean::getClassTypeSpec();
    
    return returnTypeSpec;
}

