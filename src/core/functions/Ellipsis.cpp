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

#include <sstream>


/** Constructor allowing all types; ref signals whether arguments should be values or references */
Ellipsis::Ellipsis( void ) : ArgumentRule("", TypeSpec(RbObject_name)) {
}


/** Constructor requiring a certain type specification */
Ellipsis::Ellipsis(const TypeSpec& typeSp ) : ArgumentRule("", typeSp) {
}


/** Get class vector describing type of object */
const VectorString& Ellipsis::getClass(void) const {

    static VectorString rbClass = VectorString(Ellipsis_name) + ArgumentRule::getClass();
    return rbClass;
}


/** Print value for user (in descriptions of functions, for instance */
void Ellipsis::printValue(std::ostream &o) const {

    o << argSlot.getTypeSpec() << " ...";
}

/** Provide complete information about object */
std::string Ellipsis::richInfo(void) const {

    std::ostringstream o;

    o << "Ellipsis: ";
    printValue( o );

    return o.str();
}

