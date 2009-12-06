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

#include "IntVector.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbString.h"
#include "StringVector.h"

#include <sstream>


/** Class vector describing type of object */
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
        if (value.size() != x->value.size())
            return false;
        for (size_t i=0; i<value.size(); i++) {
            if (value[i] != x->value[i])
                return false;
        }
        return true;
    }

    // Try converting the value to a string vector
    x = dynamic_cast<const StringVector*> (obj->convertTo(getType()));
    if (x == NULL)
        return false;

    bool result = true;
    if (value.size() != x->value.size())
        result = false;
    else {
        for (size_t i=0; i<value.size(); i++)
            result = result && (value[i] == x->value[i]);
    }

    delete x;
    return result;
}


/** Get element for parser */
RbObject* StringVector::getElement(const IntVector& index) const {

    if (index.size() != 0)
        throw (RbException("Index error"));
    if (index[0] >= (int)value.size() || index[0] < 0)
        throw (RbException("Index out of bound"));

    return new RbString(value[index[0]]);
}


/** Get element type for parser */
const std::string& StringVector::getElementType() const {

    return (RbNames::RbString::name);
}


/** Set element for parser; we allow resize and use "" as default value */
void StringVector::setElement(const IntVector& index, RbObject* val) {

    if (index.size() != 0)
        throw (RbException("Index error"));
    if (index[0] < 0)
        throw (RbException("Index out of bound"));

    RbString* x = dynamic_cast<RbString*>(val);
    RbString* y = NULL;
    if (x == NULL) {
        x = y = (RbString*)(val->convertTo(RbNames::RbString::name));
        if (x == NULL)
            throw (RbException("Incompatible type"));
    }

    if (index[0] >= (int)value.size())
        value.resize(index[0]);

    value[index[0]] = *x;
    delete y;
}


/** Print value for user */
void StringVector::printValue(std::ostream& o) const {

    o << "{ ";
    for (std::vector<std::string>::const_iterator i = value.begin(); i!= value.end(); i++) {
        if (i != value.begin())
            o << ", ";
        o << "\"" << (*i) << "\"";
    }
    o <<  " }";
}

/** Complete info about object */
std::string StringVector::toString(void) const {

    std::ostringstream o;
    o << "StringVector: value = ";
    printValue(o);
    
    return o.str();
}

