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
#include "RbString.h"
#include "RbUtil.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "Workspace.h"

//#include <iostream>
#include <ostream>
#include <fstream>


/** Clone object */
Func_print* Func_print::clone( void ) const {
    
    return new Func_print( *this );
}


/** Execute function */
const RbLanguageObject& Func_print::executeFunction( void ) {
    

    const std::string& f = static_cast<const RbString&>( args[0].getVariable().getValue() ).getValue();
    if ( f != "" ) {
        std::ofstream outStream;
        
        if (static_cast<const RbBoolean&>( args[1].getVariable().getValue() ).getValue()) {
            
            // open the stream to the file
            outStream.open(f.c_str(), std::fstream::out | std::fstream::app);
        }
        else {
            
            // open the stream to the file
            outStream.open(f.c_str(), std::fstream::out);
        }
        
        // print the arguments
        for (size_t i = 2; i < args.size(); i++) {
            args[i].getVariable().getValue().printValue(outStream);
        }
        outStream << std::endl;
        outStream.close();
    }
    else {
        
        std::ostream& o = std::cout;
        // print the arguments
        for (size_t i = 2; i < args.size(); i++) {
            args[i].getVariable().getValue().printValue(o);
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

