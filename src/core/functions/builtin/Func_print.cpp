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


// Definition of the static type spec member
const TypeSpec Func_print::typeSpec(Func_print_name);
const TypeSpec Func_print::returnTypeSpec(RbVoid_name);

/** Clone object */
Func_print* Func_print::clone( void ) const {
    
    return new Func_print( *this );
}


/** Execute function */
const RbLanguageObject& Func_print::executeFunction( void ) {
    

    const std::string& f = static_cast<RbString&>( (*args)[0].getValue() ).getValue();
    if ( f != "" ) {
        std::ofstream outStream;
        // open the stream to the file
        outStream.open(f.c_str(), std::fstream::out | std::fstream::app);
        
        // print the arguments
        for (size_t i = 1; i < args->size(); i++) {
            (*args)[i].getValue().printValue(outStream);
        }
        outStream << std::endl;
        outStream.close();
    }
    else {
        
        std::ostream& o = std::cout;
        // print the arguments
        for (size_t i = 1; i < args->size(); i++) {
            (*args)[i].getValue().printValue(o);
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
        argumentRules.push_back( new Ellipsis( TypeSpec(RbLanguageObject_name) ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class vector describing type of object */
const VectorString& Func_print::getClass( void ) const {
    
    static VectorString rbClass = VectorString( Func_print_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec& Func_print::getReturnType( void ) const {
    
    return returnTypeSpec;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Func_print::getTypeSpec(void) const {
    return typeSpec;
}

