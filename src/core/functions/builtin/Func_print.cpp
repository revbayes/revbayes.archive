/**
 * @file
 * This file contains the implementation of Func_print, which
 * prints the values of the arguments.
 *
 * @brief Implementation of Func_print
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @interface RbFunction
 * @package functions
 * @since Version 1.0, 2012-02-19
 *
 * $Id$
 */

#include "Ellipsis.h"
#include "Func_print.h"
#include "RbException.h"
#include "RbNullObject.h"
#include "RbUtil.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "VectorString.h"
#include "Workspace.h"

//#include <iostream>
#include <ostream>
#include <fstream>


/** Clear the arguments. We empty the list of elements to print. Then give the call back to the base class. */
void Func_print::clearArguments(void) {
    // just empty the elements list, the super smart pointers will take care of the rest
    elements.clear();
    
    // we can also set the other elements to NULL
    filename = NULL;
    append   = NULL;
}


/** Clone object */
Func_print* Func_print::clone( void ) const {
    
    return new Func_print( *this );
}


/** Execute function */
const RbLanguageObject& Func_print::executeFunction( void ) {
    

    const std::string& f = static_cast<RbString&>( filename->getValue() ).getValue();
    if ( f != "" ) {
        std::ofstream outStream;
        
        if (static_cast<RbBoolean&>( append->getValue() ).getValue()) {
            
            // open the stream to the file
            outStream.open(f.c_str(), std::fstream::out | std::fstream::app);
        }
        else {
            
            // open the stream to the file
            outStream.open(f.c_str(), std::fstream::out);
        }
        
        // print the arguments
        for (size_t i = 0; i < elements.size(); i++) {
            elements[i]->getValue().printValue(outStream);
        }
        outStream << std::endl;
        outStream.close();
    }
    else {
        
        std::ostream& o = std::cout;
        // print the arguments
        for (size_t i = 0; i < elements.size(); i++) {
            elements[i]->getValue().printValue(o);
        }
        o << std::endl;
    }
    
    
    return RbNullObject::getInstance();
}


/** Get argument rules */
const ArgumentRules& Func_print::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ValueRule( "filename", new RbString("") ) );
        argumentRules.push_back( new ValueRule( "append", new RbBoolean(false) ) );
        argumentRules.push_back( new Ellipsis( RbLanguageObject::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_print::getClassName(void) { 
    
    static std::string rbClassName = "Print function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_print::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_print::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_print::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RbVoid_name;
    return returnTypeSpec;
}


/** We catch here the setting of the argument variables to store our parameters. */
void Func_print::setArgumentVariable(std::string const &name, const RbVariablePtr& var) {
    
    if ( name == "filename" ) {
        filename = var;
    }
    else if ( name == "append" ) {
        append = var;
    }
    else if ( name == "" ) {
        elements.push_back(var);
    }
    else {
        RbFunction::setArgumentVariable(name, var);
    }
}

