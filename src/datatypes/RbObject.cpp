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
#include "VectorString.h"

#include <sstream>


/** Brief info about object: use static class attribute */
std::string RbObject::briefInfo(void) const {

    std::ostringstream o;
    o << "Object of type ";
    getClass().printValue(o);
    return o.str();
}


/** Convert to type: throw an error */
RbObject* RbObject::convert(const std::string& type) const {

    if (getType() == type) {
        return this->clone();
    }
    else {
        return convertTo(type);
    }
}


/** Get class vector describing type of object */
const VectorString& RbObject::getClass(void) const { 

    static VectorString rbClass = VectorString(RbObject_name);
	return rbClass; 
}

/** Get type (first entry in class vector) */
const std::string& RbObject::getType(void) const {

    return getClass()[0];
}


/** Is the object convertible to type? */
bool RbObject::isConvertible(const std::string& type) const {

    if (getType().compare(type) == 0)
        return true;
    else
        return isConvertibleTo(type);
}


/** Are we of specified type? We need to check entire class vector in case we are derived from type. */
bool RbObject::isType(const std::string& type) const {

    const VectorString& classVec = getClass();

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

/** Make sure we can print the value of the object easily */
std::ostream& operator<<(std::ostream& o, const RbObject& x) {

    x.printValue(o);
    return o;
}

