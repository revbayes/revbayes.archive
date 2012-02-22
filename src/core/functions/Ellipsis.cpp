/**
 * @file
 * This file contains the implementation of Ellipsis, which is
 * used to describe a dot dot dot formal (...).
 *
 * @brief Implementation of Ellipsis 
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

#include "DAGNode.h"
#include "Ellipsis.h"
#include "RbUtil.h"
#include "RbObject.h"
#include "TypeSpec.h"
#include "VectorString.h"



/** Constructor allowing all types; ref signals whether arguments should be values or references */
Ellipsis::Ellipsis( void ) : ArgumentRule("", RbObject::getClassTypeSpec() ) {
}


/** Constructor requiring a certain type specification */
Ellipsis::Ellipsis(const TypeSpec& typeSp ) : ArgumentRule("", typeSp) {
}


/** Get class name of object */
const std::string& Ellipsis::getClassName(void) { 
    
    static std::string rbClassName = "Ellipsis";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Ellipsis::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( ArgumentRule::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Ellipsis::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Print value for user (in descriptions of functions, for instance */
void Ellipsis::printValue(std::ostream &o) const {

    o << argSlot.getTypeSpec() << " ...";
}

