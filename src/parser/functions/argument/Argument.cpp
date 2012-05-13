/**
 * @file
 * This file contains the implementation of Argument, which is
 * used to hold a potentially labeled argument passed to a
 * function.
 *
 * @brief Implementation of Argument
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 *
 * $Id$
 */

#include <sstream>

#include "Argument.h"
#include "DAGNode.h"
#include "RbUtil.h"


/** Construct from argument label and DAG node */
Argument::Argument(const std::string& argLabel) : RbInternal() {

    label   = argLabel;
    
}


/** Get class name of object */
const std::string& Argument::getClassName(void) { 
    
    static std::string rbClassName = "Argument";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Argument::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbInternal::getClassTypeSpec() ) );
    
	return rbClass; 
}


const std::string& Argument::getLabel( void ) const {
    return label;
}

