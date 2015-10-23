#include "Argument.h"
#include "ArgumentRule.h"
#include "Func_source.h"
#include "Parser.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RevNullObject.h"
#include "RlString.h"
#include "RlUtils.h"
#include "TypeSpec.h"
#include "RlUserInterface.h"
#include "Workspace.h"

#include <fstream>

using namespace RevLanguage;

/** Default constructor */
Func_source::Func_source( void ) : Procedure()
{
    
}


/** Clone object */
Func_source* Func_source::clone( void ) const
{
    
    return new Func_source( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_source::execute( void )
{
    
    /* Open file */
    std::string fname = static_cast<const RlString &>( args[0].getVariable()->getRevObject() ).getValue();
    std::ifstream inFile( fname.c_str() );
    
    
    bool echo_on = static_cast<const RlBoolean &>( args[1].getVariable()->getRevObject() ).getValue();
    
    if ( !inFile )
        throw RbException( "Could not open file \"" + fname + "\"" );
    
    /* Initialize */
    std::string commandLine;
    int lineNumber = 0;
    int result = 0;     // result from processing of last command
    RBOUT("Processing file \"" + fname + "\"");
    
    /* Command-processing loop */
    while ( inFile.good() )
    {
        
        // Read a line
        std::string line;
        getline( inFile, line );
        lineNumber++;
        
        if (echo_on)
        {
            
            if ( result == 1 )
            {
                std::cout << "+ " << line;
            }
            else
            {
                std::cout << "> " << line;
            }
            
        }
        
        // If previous result was 1 (append to command), we do this
        if ( result == 1 )
        {
            commandLine += line;
        }
        else
        {
            commandLine = line;
        }
            
        // Process the line and record result
        result = Parser::getParser().processCommand( commandLine, &Workspace::userWorkspace() );
        if ( result == 2 )
        {
            std::ostringstream msg;
            msg << "Problem processing line " << lineNumber << " in file \"" << fname << "\"";
            throw RbException( msg.str() );
        }
        
    }
    
    /* Return control */
    RBOUT("Processing of file \"" + fname + "\" completed");
    
    return NULL;
}


/** Get argument rules */
const ArgumentRules& Func_source::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "file"   , RlString::getClassTypeSpec() , "The name of the file to read-in.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
        argumentRules.push_back( new ArgumentRule( "echo.on", RlBoolean::getClassTypeSpec(), "Should we print the commands from the file on the screen?", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(false) ) );
        rulesSet = true;
    
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_source::getClassType(void) {
    
    static std::string revType = "Func_source";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_source::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}

/** Get type spec */
const TypeSpec& Func_source::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_source::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RlUtils::Void;
    
    return returnTypeSpec;
}

