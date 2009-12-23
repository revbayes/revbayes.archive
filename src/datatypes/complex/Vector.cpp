/**
 * @file
 * This file contains the implementation of Vector, a complex type
 * used to hold double vectors.
 *
 * @brief Implementation of Vector
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

#include "ContainerIterator.h"
#include "IntVector.h"
#include "RbDouble.h"
#include "RbException.h"
#include "RbNames.h"
#include "StringVector.h"
#include "Vector.h"

#include <sstream>


/** Construct vector with one double x */
Vector::Vector(double x) {

    value.push_back(x);
}


/** Construct vector with n doubles x */
Vector::Vector(int n, double x) {

    for (double i = 0; i < n; i++)
        value.push_back(x);
}


/** Constructor from double vector */
Vector::Vector(std::vector<double>& x) {

    value = x;
}


/** Clone function */
Vector* Vector::clone() const {

    return new Vector(*this);
}


/** Pointer-based equals comparison */
bool Vector::equals(const RbObject* obj) const {

    // Use built-in fast down-casting first
    const Vector* p = dynamic_cast<const Vector*> (obj);
    if (p != NULL) {
        if (value.size() != p->value.size())
            return false;
        for (size_t i = 0; i < value.size(); i++) {
            if (value[i] != p->value[i])
                return false;
        }
        return true;
    }

    // Try converting the value to a double vector
    p = dynamic_cast<const Vector*> (obj->convertTo(getType()));
    if (p == NULL)
        return false;

    bool result = true;
    if (value.size() == p->value.size()) {
        for (size_t i = 0; i < value.size(); i++)
            result = result && (value[i] == p->value[i]);
    }
    else
        result = false;

    delete p;
    return result;
}


/** Get class vector describing type of object */
const StringVector& Vector::getClass() const {

    static StringVector rbClass = StringVector(Vector_name) + RbComplex::getClass();
    return rbClass;
}


/** Get atomic class */
const StringVector& Vector::getAtomicClass(void) const {

    static RbDouble x = RbDouble(0.0);
    return x.getClass();
}


/** Get element for parser (read-only) */
const RbObject* Vector::getElement(const IntVector& index) const {

    static RbDouble x = RbDouble(0.0);

    if (index.size() != 1)
        throw (RbException("Index error"));
    if (index[0] >= (double)value.size() || index[0] < 0)
        throw (RbException("Index out of bound"));

    x.setValue(value[index[0]]);
    return &x;
}


/** Get element length for parser */
const IntVector& Vector::getElementLength(void) const {

    static IntVector length = IntVector(0);

    length[0] = int(value.size());
    return length;
}


/** Tell the parser it cannot modify the elements */
RbObject* Vector::getElementRef(const IntVector& index) {

    throw (RbException("Elements are not modifiable"));
}

/** Tell the parser it cannot modify the elements */
std::vector<double>& Vector::getValue(void) {

    return value;
}



/** Allow the parser to resize the vector */
void Vector::resize(const IntVector& len) {

    if (len.size() != 1 || len[0] < 0)
        throw (RbException("Length specification error"));

    value.resize(len[0]);
}


/** Allow parser to set an element (any type conversion is done by parser) */
void Vector::setElement(const IntVector& index, RbObject* val) {

    if ( !val->isType(RbDouble_name) )
        throw (RbException("Type mismatch"));

    if ( index.size() != 1 || index[0] < 1 )
        throw (RbException("Index error"));

    // Do we want to allow resize to fit the new element or throw an error?
    // If we resize, then we have to fill in elements with some default value
    // or alternatively, keep a vector of bools signifying which elements should
    // be considered null elements. Maybe we could use nan.
    if ( index[0] >= int(value.size()) ) {
        int oldLen = int(value.size());
        resize(index[0]);
        for (int i=oldLen; i<index[0]; i++)
            value[i] = 0;
    }

    value[index[0]] = ((RbDouble*)(val))->getValue();
}


/** Allow parser to rearrange the container (actually do not allow it) */
void Vector::setElementLength(const IntVector& len) {

    if ( len.size() != 1 && len[0] != int(value.size()) )
        throw (RbException("Length specification error"));
}


/** Print value for user */
void Vector::printValue(std::ostream& o) const {

    o << "{ ";
    for (std::vector<double>::const_iterator i = value.begin(); i!= value.end(); i++) {
        if (i != value.begin())
            o << ", ";
        o << (*i);
    }
    o <<  " }";
}


/** Complete info about object */
std::string Vector::toString(void) const {

    std::ostringstream o;
    o <<  "Vector: value = ";
    printValue(o);

    return o.str();
}

