/**
 * @file
 * This file contains the implementation of IntVector, a complex type
 * used to hold int vectors.
 *
 * @brief Implementation of IntVector
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

#include "IntVector.h"
#include "RbException.h"
#include "RbNames.h"
#include "StringVector.h"

#include <iostream>
#include <sstream>

/** Vector of class names */
const StringVector IntVector::rbClass = StringVector(RbNames::IntVector::name) + RbComplex::rbClass;


/** Construct vector with one int x */
IntVector::IntVector(int x) {

    value.push_back(x);
}

/** Construct vector with n ints x */
IntVector::IntVector(int n, int x) {

    for (int i = 0; i < n; i++)
        value.push_back(x);
}


/** Constructor from int vector */
IntVector::IntVector(std::vector<int>& x) {

    value = x;
}


/** Clone function */
RbObject* IntVector::clone() const {

    return (RbObject*)(new IntVector(*this));
}

/** Pointer-based equals comparison */
bool IntVector::equals(const RbObject* obj) const {

    // Use built-in fast down-casting first
    const IntVector* x = dynamic_cast<const IntVector*> (obj);
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

    // Try converting the value to an int vector
    x = dynamic_cast<const IntVector*> (obj->convertTo(getType()));
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
void IntVector::printValue(std::ostream& o) const {

    o << "[";
    for (std::vector<int>::const_iterator i = value.begin(); i!= value.end(); i++) {
        if (i != value.begin())
            o << ", ";
        o << "\"" << (*i) << "\"";
    }
    o <<  "]";
}

/** Complete info about object */
std::string IntVector::toString(void) const {

    std::ostringstream o;
    o <<  "IntVector; value = ";
    printValue(o);

    return o.str();
}

