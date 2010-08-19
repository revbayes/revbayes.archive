/**
 * @file
 * This file contains the implementation of RbString, which is
 * a REvBayes wrapper around a regular string.
 *
 * @brief Implementation of RbString
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The REvBayes development core team
 * @license GPL version 3
 *
 * $Id$
 */
 
#include "RbException.h"
#include "RbNames.h"
#include "RbString.h"
#include "VectorString.h"


/** Constructor from string */
RbString::RbString(const std::string& v) 
    : RbComplex(), value(v) {
}


/** Type conversion to string */
RbString::operator std::string() const {

    return value;
}


/** String concatenation with operator+  */
RbString RbString::operator+(const RbString& s) const {

    return RbString(value+s.value);
}

/** Clone function */
RbString* RbString::clone() const {

	return new RbString(*this);
}


/** Convert to object of another class. The caller manages the object. */
RbObject* RbString::convertTo(const std::string& type) const {

    /*if (type == Nucleotide_name)
        return new Nucleotide(value[0]);
    throw RbException("Cannot convert string to " + type + ".");*/
    return NULL;
}

/** Equals comparison */
bool RbString::equals(const RbObject* obj) const {

    const RbString* p = dynamic_cast<const RbString*>(obj);
    if (p == NULL)
        return false;

	return (p->value == value);
}


/** Get class vector describing type of object */
const VectorString& RbString::getClass() const {

    static VectorString rbClass = VectorString(RbString_name) + RbComplex::getClass();
    return rbClass;
}

/** Is the object convertible to an object of another class type? */
bool RbString::isConvertibleTo(const std::string& type) const {

    return false;
}

/** Print value */
void RbString::printValue(std::ostream& o) const {

	o << "\"" << value << "\"";
}


/** Complete info on object */
std::string RbString::toString() const {

	return "RbString; value = '" + value + "'";
}


