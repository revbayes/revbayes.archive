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
#include "StringVector.h"

const StringVector RbString::rbClass = StringVector(RbNames::String::name) + RbComplex::rbClass;


/** Constructor from string */
RbString::RbString(const std::string& v) 
    : RbComplex(), value(v) {
}


/** Copy constructor */
RbString::RbString(const RbString& s) :
    RbComplex(), value(s.value) {
}


/** Clone function */
RbObject* RbString::clone() const {

	return (RbObject*)(new RbString(*this));
}


/** Equals comparison */
bool RbString::equals(const RbObject* obj) const {

    const RbString* p = dynamic_cast<const RbString*>(obj);
    if (p == NULL)
        return false;

	return (p->value == value);
}


/** Print value */
void RbString::printValue(std::ostream& o) const {

	o << value << std::endl;
}


/** Complete info on object */
std::string RbString::toString() const {

	return "RbString; value = '" + value + "'";
}


/** Type conversion to string */
RbString::operator std::string() const {

    return value;
}

/** Add strings together */
RbString RbString::operator+(const RbString& s) const {

    return RbString(value+s.value);
}

RbObject& RbString::operator=(const RbObject& obj) {

    try {
        // Use built-in fast down-casting first
        const RbString& x = dynamic_cast<const RbString&> (obj);

        RbString& y = (*this);
        y = x;
        return y;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const RbString& x = dynamic_cast<const RbString&> (*(obj.convertTo(RbNames::String::name)));

            RbString& y = (*this);
            y = x;
            return y;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0] + " to " + RbNames::String::name);
            throw e;
        }
    }

    // dummy return
    return (*this);
}

RbString& RbString::operator=(const RbString& ar) {

    value = ar.value;
    return (*this);
}

