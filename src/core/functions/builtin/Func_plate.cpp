/**
 * @file
 * This file contains the implementation of Func_plate.
 *
 * @brief Implementation of Func_sqrt
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-03-10 12:55:11 +0100 (Sat, 10 Mar 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id: Func_plate.cpp 1327 2012-03-10 11:55:11Z hoehna $
 */

#include "Ellipsis.h"
#include "Func_plate.h"
#include "RbException.h"
#include "RbNullObject.h"
#include "RbUtil.h"
#include "RbVector.h"
#include "TypeSpec.h"
#include "ValueRule.h"

#include <cassert>
#include <cmath>


Func_plate::Func_plate(void) : RbFunction() {
    
}


/** Clone object */
Func_plate* Func_plate::clone( void ) const {
    
    return new Func_plate( *this );
}


/** Execute function */
const RbLanguageObject& Func_plate::execute( void ) {
    
    // get the command string
    std::string command = static_cast<const RbString &>( args[0].getVariable().getValue() ).getValue();
    
    std::cerr << command << std::endl;
    
    std::vector<const Container *> ranges;
    for (std::vector<Argument>::const_iterator i = args.begin()+1; i != args.end(); ++i) {
        ranges.push_back( static_cast<const Container *>( i->getVariable().getValue().clone() ) );
        ranges[ranges.size()-1]->printValue( std::cerr );
        std::cerr << std::endl;
    }
    
    
    
    return RbNullObject::getInstance();
}


/** Get argument rules */
const ArgumentRules& Func_plate::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if (!rulesSet) 
    {
        argumentRules.push_back( new ValueRule( "command", RbString::getClassTypeSpec() ) );
        argumentRules.push_back( new Ellipsis( Container::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_plate::getClassName(void) { 
    
    static std::string rbClassName = "plate function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_plate::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_plate::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = TypeSpec(RbVoid_name);
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_plate::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RealPos::getClassTypeSpec();
    return returnTypeSpec;
}

