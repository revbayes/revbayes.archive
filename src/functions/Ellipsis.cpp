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
#include "RbNames.h"
#include "RbObject.h"
#include "TypeSpec.h"
#include "VectorString.h"

#include <sstream>


/** Constructor allowing all types and dims */
Ellipsis::Ellipsis(void)
    : ArgumentRule("", TypeSpec("")) {
}


/** Constructor requiring a certain type and dim */
Ellipsis::Ellipsis(const std::string& type, int dim)
    : ArgumentRule("", TypeSpec(type, dim)) {
}


/** Get class vector describing type of object */
const VectorString& Ellipsis::getClass(void) const {

    static VectorString rbClass = VectorString(Ellipsis_name) + ArgumentRule::getClass();
    return rbClass;
}


/** Check validity of argument. Call it on a single argument if it is used! */
bool Ellipsis::isArgValid(DAGNode* var) const {

    if (!var->getValue()->isType(argSlot.getTypeSpec().getType()))
        return false;

    return true;
}


/** Print value for user (in descriptions of functions, for instance */
void Ellipsis::printValue(std::ostream &o) const {

    o << "...";
}

/** Provide complete information about object */
std::string Ellipsis::richInfo(void) const {

    std::ostringstream o;

    o << "Ellipsis: ";

    return o.str();
}

