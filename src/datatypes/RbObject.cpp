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
std::string RbObject::briefInfo() const {

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

const StringVector& RbObject::getClass(void) const { 

	return rbClass; 
}

/** Get element */
RbObject* RbObject::getElement(const IntVector& index) const {

    throw (RbException("Object does not have elements"));
    return NULL;    // Dummy return
}


/** Get element length in each dimension */
const IntVector& RbObject::getElementLength() const {

    throw (RbException("Object does not have elements"));
}


/** Get element type */
const std::string& RbObject::getElementType() const {

    throw (RbException("Object does not have elements"));
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


/** Set element */
void RbObject::setElement(const IntVector& index, RbObject* val) {

    throw (RbException("Object does not have elements"));
}


/** Print info about object simply by using toString function */
void RbObject::print(std::ostream& o) const {

    o << toString() << std::endl;
}

