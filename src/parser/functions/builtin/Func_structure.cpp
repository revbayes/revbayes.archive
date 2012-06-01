/**
 * @file
 * This file contains the implementation of Func_structure, which is used
 * to print info about the structure of a variable.
 *
 * @brief Implementation of Func_structure
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id$
 */

#include "ConstArgumentRule.h"
#include "DAGNode.h"
#include "DeterministicNode.h"
#include "Func_structure.h"
#include "Integer.h"
#include "RbException.h"
#include "RbNullObject.h"
#include "RbUtil.h"
#include "StochasticNode.h"
#include "TypeSpec.h"

#include <cassert>


/** Clone object */
Func_structure* Func_structure::clone( void ) const {

    return new Func_structure( *this );
}


/** Execute function */
RlValue<RbLanguageObject> Func_structure::execute( void ) {

    args[0]->getVariable()->getDagNode()->printStruct( std::cout );

    return RbPtr<RbLanguageObject>( NULL );
}


/** Get argument rules */
const ArgumentRules& Func_structure::getArgumentRules( void ) const {

    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;

    if ( !rulesSet ) {

        argumentRules.push_back( new ArgumentRule( "variable", true, RbObject::getClassTypeSpec() ) );
        rulesSet = true;
    }

    return argumentRules;
}


/** Get class name of object */
const std::string& Func_structure::getClassName(void) { 
    
    static std::string rbClassName = "Structure function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_structure::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_structure::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_structure::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RbVoid_name;
    return returnTypeSpec;
}

