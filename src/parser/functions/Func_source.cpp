/**
 * @file
 * This file contains the implementation of Func_source, which is
 * the function used to read commands (source) from a file.
 *
 * @brief Implementation of Func_source
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @interface RbFunction
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id$
 */

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
#include "ValueRule.h"
#include "VectorString.h"
#include "Workspace.h"

#include <fstream>


// Definition of the static type spec member
const TypeSpec Func_source::typeSpec(Func_source_name);
const TypeSpec Func_source::returnTypeSpec(RbVoid_name);

/** Clone object */
Func_source* Func_source::clone( void ) const {

    return new Func_source( *this );
}


/** Execute function */
const RbLanguageObject& Func_source::executeFunction( void ) {

    /* Open file */
    std::string fname = static_cast<RbString&>( filename->getValue() ).getValue();
    std::ifstream inFile( fname.c_str() );
    
    
    bool echo_on = static_cast<RbBoolean&>( echo->getValue() ).getValue();
    
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
        result = Parser::getParser().processCommand( commandLine );
        if ( result == 2 ) {
            std::ostringstream msg;
            msg << "Problem processing line " << lineNumber << " in file \"" << fname << "\"";
            throw RbException( msg.str() );
        }
    }

    /* Return control */
    RBOUT("Processing of file \"" + fname + "\" completed");

    return RbNullObject::getInstance();
}


/** Get argument rules */
const ArgumentRules& Func_source::getArgumentRules( void ) const {

    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;

    if ( !rulesSet ) {

        argumentRules.push_back( new ValueRule( "file", RbString_name ) );
        argumentRules.push_back( new ValueRule( "echo.on", RbBoolean_name, new RbBoolean(false) ) );
        rulesSet = true;
    }

    return argumentRules;
}


/** Get class vector describing type of object */
const VectorString& Func_source::getClass( void ) const {

    static VectorString rbClass = VectorString( Func_source_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec& Func_source::getReturnType( void ) const {

    return returnTypeSpec;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Func_source::getTypeSpec(void) const {
    return typeSpec;
}


/** We catch here the setting of the argument variables to store our parameters. */
void Func_source::setArgumentVariable(std::string const &name, const RbVariablePtr& var) {
    
    if ( name == "file" ) {
        filename = var;
    }
    else if ( name == "echo.on" ) {
        echo = var;
    }
    else {
        RbFunction::setArgumentVariable(name, var);
    }
}

