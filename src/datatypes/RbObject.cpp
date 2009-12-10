/**
 * @file
 * This file contains the implementation of some of the functions
 * in the abstract base class for language objects, RbObject.
 *
 * @brief Partial implementation of RbObject
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-02, version 1.0
 * @extends RbObject
 *
 * $Id$
 */

#include "RbException.h"
#include "RbNames.h"
#include "RbObject.h"
#include "StringVector.h"

#include <sstream>


/** Brief info about object: use static class attribute */
std::string RbObject::briefInfo(void) const {

    std::ostringstream o;
    o << "Object of type ";
    getClass().printValue(o);
    return o.str();
}


/** Convert to type: throw an error */
RbObject* RbObject::convertTo(const std::string& type) const {

    throw (RbException("Conversion to " + type + " not supported"));
    return NULL;    // Dummy return
}


/** Get class vector describing type of object */
const StringVector& RbObject::getClass(void) const { 

    static StringVector rbClass = StringVector(RbNames::RbObject::name);
	return rbClass; 
}


/** Get atomic type (first entry in atomic class vector, describing type beyond which subscripts do not work) */
const std::string& RbObject::getAtomicType(void) const {

    return getAtomicClass()[0];
}


/** Get type (first entry in class vector) */
const std::string& RbObject::getType(void) const {

    return getClass()[0];
}


/** Is convertible to type? Default is false */
bool RbObject::isConvertibleTo(const std::string& type) const {

    return false;
}


/** Are we of specified type? We need to check entire class vector in case we are derived from type. */
bool RbObject::isType(const std::string& type) const {

    const StringVector& classVec = getClass();

    for (size_t i=0; i<classVec.size(); i++) {
        if (type == classVec[i])
            return true;
    }

	return false;
}


/** Print info about object simply by using toString function */
void RbObject::print(std::ostream& o) const {

    o << toString() << std::endl;
}

