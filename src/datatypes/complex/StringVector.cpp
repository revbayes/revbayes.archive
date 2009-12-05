/**
 * @file
 * This file contains the implementation of StringVector, a complex type
 * used to hold string vectors.
 *
 * @brief Implementation of StringVector
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 * @extends RbComplex
 *
 * $Id$
 */

#include "StringVector.h"
#include "RbException.h"
#include "RbNames.h"

#include <iostream>


/** Vector of class names */
const StringVector StringVector::rbClass = StringVector(RbNames::StringVector::name) + RbComplex::rbClass;


/** Construct empty vector of length n */
StringVector::StringVector(int n) {

    value.resize(n);
}

/** Construct vector with one string x */
StringVector::StringVector(std::string x) {

    value.push_back(x);
}

/** Construct vector with n strings x */
StringVector::StringVector(int n, std::string x) {

    for (int i = 0; i < n; i++)
        value.push_back(x);
}

/** Copy constructor */
StringVector::StringVector(const StringVector& x) {

    value = x.value;
}

/** Constructor from std::string vector */
StringVector::StringVector(std::vector<std::string>& x) {

    value = x;
}

/** Base class assignment; this probably slices content */
RbObject& StringVector::operator=(const RbObject& obj) {

    try {
        // Use built-in fast down-casting first
        const StringVector& x = dynamic_cast<const StringVector&> (obj);

        StringVector& sv = (*this);
        sv = x;
        return sv;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const StringVector& x = dynamic_cast<const StringVector&> (*(obj.convertTo("StringVector")));

            StringVector& sv = (*this);
            sv = x;
            return sv;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0] + " to StringVector");
            throw e;
        }
    }

    // dummy return
    return (*this);
}


/** Regular assignment */
StringVector& StringVector::operator=(const StringVector& x) {

    if (this != &x)
        value = x.value;

    return (*this);
}

/** Concatenation with operator+ */
StringVector StringVector::operator+(const StringVector& x) const {

    StringVector tempVec = *this;
    for (size_t i = 0; i < x.value.size(); i++)
        tempVec.push_back(x.value[i]);

    return tempVec;
}

/** Clone function */
RbObject* StringVector::clone() const {

    return (RbObject*)(new StringVector(*this));
}

/** Pointer-based equals comparison */
bool StringVector::equals(const RbObject* obj) const {

    // Use built-in fast down-casting first
    const StringVector* x = dynamic_cast<const StringVector*> (obj);
    if (x != NULL) {
        if (value.size() == x->value.size()) {
            for (size_t i = 0; i < value.size(); i++) {
                if (value[i] != x->value[i])
                    return false;
            }
            return true;
        }
        else
            return false;
    }

    // Try converting the value to a string vector
    x = dynamic_cast<const StringVector*> (obj->convertTo(getType()));
    if (x == NULL)
        return false;

    bool result = true;
    if (value.size() == x->value.size()) {
        for (size_t i = 0; i < value.size(); i++)
            result = result && (value[i] == x->value[i]);
    }
    else
        result = false;

    delete x;
    return result;
}

/** Print value for user */
void StringVector::printValue(std::ostream& o) const {

    o << "[";
    for (std::vector<std::string>::const_iterator i = value.begin(); i!= value.end(); i++) {
        if (i != value.begin())
            o << ", ";
        o << "\"" << (*i) << "\"";
    }
    o <<  "]";
}

/** Complete info about object */
std::string StringVector::toString(void) const {

    std::string tempStr = "StringVector; value = [";
    for (std::vector<std::string>::const_iterator i = value.begin(); i!= value.end(); i++) {
        if (i != value.begin())
            tempStr += ", ";
        tempStr += ("\"" + (*i) + "\"");
    }
    tempStr += "]";

    return tempStr;
}

