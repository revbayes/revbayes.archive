#include "Argument.h"
#include "ArgumentRule.h"
#include "Func_ls.h"
#include "RbException.h"
#include "RlBoolean.h"
#include "RlUtils.h"
#include "TypeSpec.h"
#include "RlUserInterface.h"
#include "Workspace.h"

#include <fstream>

using namespace RevLanguage;

/** Default constructor */
Func_ls::Func_ls( void ) : Procedure()
{
    
}


/** Clone object */
Func_ls* Func_ls::clone( void ) const
{
    
    return new Func_ls( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_ls::execute( void )
{
    
    bool printAll = static_cast<const RlBoolean &>( args[0].getVariable()->getRevObject() ).getValue();
    
    std::stringstream s;

    VariableTable variables = VariableTable();
    
    if ( printAll )
    {
        const VariableTable& globalVars = Workspace::globalWorkspace().getVariableTable();
        variables.insert( globalVars.begin(), globalVars.end() );
    }
    
    const VariableTable& userVars = env->getVariableTable();
    variables.insert( userVars.begin(), userVars.end() );
    
    if ( !variables.empty() )
    {
        RBOUT( "\n" );
        RBOUT( "Variable table:" );
        RBOUT( "===============" );
        
        s.str("");
        VariableTable::const_iterator it;
        std::ostringstream t1, t2;
        for ( it = variables.begin(); it != variables.end(); ++it )
        {
            t1.str("");
            t1 << (*it).first << " = ";
            t2.str("");
            (*it).second->printValue( t2, true );
            s << t1.str() << StringUtilities::oneLiner( t2.str(), 75 - t1.str().size() ) << std::endl;
        }
        
        s << std::endl;
        RBOUT( s.str() );
    }
    
    s.str("");
    Workspace::userWorkspace().getFunctionTable().printValue( s, printAll );
    if ( s.str().size() > 0 )
    {
        RBOUT( "\n" );
        RBOUT( "Function table:" );
        RBOUT( "===============" );
        
        s << std::endl;
        RBOUT( s.str() );
    }
    
    TypeTable types = TypeTable();

    if ( printAll )
    {
        const TypeTable& globalTypes = Workspace::globalWorkspace().getTypeTable();
        types.insert( globalTypes.begin(), globalTypes.end() );
    }
    
    const TypeTable& userTypes = Workspace::userWorkspace().getTypeTable();
    types.insert( userTypes.begin(), userTypes.end() );
    
    if ( !types.empty() )
    {
        RBOUT( "\n" );
        RBOUT( "Type table:" );
        RBOUT( "===========" );
        
        s.str("");
        TypeTable::const_iterator it;
        std::ostringstream t;
        for ( it = types.begin(); it != types.end(); ++it )
        {
            t.str("");
            t << (*it).first << " = ";
            t << (*it).second->getTypeSpec();
            s << StringUtilities::oneLiner( t.str(), 75 ) << std::endl;
        }
        
        s << std::endl;
        RBOUT( s.str() );
        
    }
    
    return NULL;
}


/** Get argument rules */
const ArgumentRules& Func_ls::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "all", RlBoolean::getClassTypeSpec(), "Should we print all variables and functions including provided ones by RevBayes?", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(false) ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_ls::getClassType(void)
{
    
    static std::string revType = "Func_ls";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_ls::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/**
 * Get the primary Rev name for this function.
 */
const std::string& Func_ls::getFunctionName( void ) const
{
    // create a static name variable that is the same for all instance of this class
    static std::string f_name = "ls";
    
    return f_name;
}


/** Get type spec */
const TypeSpec& Func_ls::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_ls::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = RlUtils::Void;
    
    return returnTypeSpec;
}

