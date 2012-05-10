/**
 * @file
 * This file contains the implementation of Func_source, which is
 * the function used to read commands (source) from a file.
 *
 * @brief Implementation of Func_source
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

#include "ConstArgumentRule.h"
#include "DAGNode.h"
#include "Func_source.h"
#include "Model.h"
#include "Parser.h"
#include "RbException.h"
#include "RbNullObject.h"
#include "RbUtil.h"
#include "RbString.h"
#include "TypeSpec.h"
#include "UserInterface.h"
#include "Workspace.h"

#include <fstream>



/** Default constructor */
Func_source::Func_source( void ) : RbFunction() {

}


/** Clone object */
Func_source* Func_source::clone( void ) const {

    return new Func_source( *this );
}


/** Execute function */
RbPtr<RbLanguageObject> Func_source::executeFunction( const std::vector<const RbObject*>& args ) {

    /* Open file */
    std::string fname = static_cast<const RbString*>( args[0] )->getValue();
    std::ifstream inFile( fname.c_str() );
    
    
    bool echo_on = static_cast<const RbBoolean*>( args[1] )->getValue();
    
    if ( !inFile )
        throw RbException( "Could not open file \"" + fname + "\"" );

    /* Initialize */
    std::string commandLine;
    int lineNumber = 0;
    int result = 0;     // result from processing of last command
    RBOUT("Processing file \"" + fname + "\"");

    /* Command-processing loop */
    while ( inFile.good() ) {

        // Read a line
        std::string line;
        getline( inFile, line );
        lineNumber++;

        if (echo_on) RBOUT(line);

        // If previous result was 1 (append to command), we do this
        if ( result == 1 )
            commandLine += line;
        else
            commandLine = line;

        // Process the line and record result
        result = Parser::getParser().processCommand( commandLine, &Workspace::userWorkspace() );
        if ( result == 2 ) {
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
const ArgumentRules& Func_source::getArgumentRules( void ) const {

    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;

    if ( !rulesSet ) {

        argumentRules.push_back( new ConstArgumentRule( "file", RbString::getClassTypeSpec() ) );
        argumentRules.push_back( new ConstArgumentRule( "echo.on", RbBoolean::getClassTypeSpec(), new RbBoolean(false) ) );
        rulesSet = true;
    }

    return argumentRules;
}


/** Get class name of object */
const std::string& Func_source::getClassName(void) { 
    
    static std::string rbClassName = "Source function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_source::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_source::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_source::getReturnType( void ) const {

    static TypeSpec returnTypeSpec = RbVoid_name;
    
    return returnTypeSpec;
}

