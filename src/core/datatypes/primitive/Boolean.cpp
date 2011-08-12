/**
 * @file
 * This file contains the implementation of Boolean, which is
 * a RevBayes wrapper around a regular bool.
 *
 * @brief Implementation of Boolean
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 * @extends RbObject
 *
 * $Id$
 */


#include "Boolean.h"
#include "Real.h"
#include "Integer.h"
#include "RbNames.h"
#include "TypeSpec.h"
#include "VectorString.h"

#include <sstream>


/** Default constructor */
Boolean::Boolean(void) : RbLanguageObject(), value(false) {

}

/** Construct from bool */
Boolean::Boolean(const bool v) : RbLanguageObject(), value(v) {

}


/** Get brief info about object */
std::string Boolean::briefInfo(void) const {
    
	std::ostringstream o;
    o << "Boolean: value = ";
    printValue(o);
    return o.str();
}

/** Clone object */
Boolean* Boolean::clone(void) const {

	return  new Boolean(*this);
}


/** Convert to type and dim. The caller manages the returned object. */
RbLanguageObject* Boolean::convertTo(const std::string& type) const {

    if (type == Integer_name) {
        if (value)
            return new Integer(1);
        else 
            return new Integer(0);
    }
    else if (type == Real_name) {
        if (value)
            return new Real(1.0);
        else 
            return new Real(0.0);
    }

    return RbLanguageObject::convertTo(type);
}


/** Get class vector describing type of object */
const VectorString& Boolean::getClass() const {

    static VectorString rbClass = VectorString(Boolean_name) + RbObject::getClass();
    return rbClass;
}


/** Is convertible to type and dim? */
bool Boolean::isConvertibleTo(const std::string& type, bool once) const {

    if (type == Integer_name)
        return true;
    else if (type == Real_name)
        return true;

    return RbObject::isConvertibleTo(type, once);
}


/** Print value for user */
void Boolean::printValue(std::ostream &o) const {

    o << (value ? "true" : "false");
}

/** Get complete info about object */
std::string Boolean::richInfo(void) const {

	std::ostringstream o;
    o << "Boolean: value = ";
    printValue(o);
    return o.str();
}

