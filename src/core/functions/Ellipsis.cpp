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



// Definition of the static type spec member
const TypeSpec Ellipsis::typeSpec(Ellipsis_name);

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


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Ellipsis::getTypeSpec(void) const {
    return typeSpec;
}


/** Print value for user (in descriptions of functions, for instance */
void Ellipsis::printValue(std::ostream &o) const {

    o << argSlot.getTypeSpec() << " ...";
}

