/**
 * @file
 * This file contains the implementation of VectorBoolean, a complex type
 * used to hold int vectors.
 *
 * @brief Implementation of VectorBoolean
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

#include "Boolean.h"
#include "ContainerIterator.h"
#include "VectorBoolean.h"
#include "RbException.h"
#include "Integer.h"
#include "RbNames.h"
#include "VectorInteger.h"
#include "VectorString.h"

#include <sstream>



/** Default constructor */
VectorBoolean::VectorBoolean(void) : Vector() {

}


/** Construct vector with one int x */
VectorBoolean::VectorBoolean(bool x) : Vector() {

    value.push_back(x);
}


/** Construct vector with n ints x */
VectorBoolean::VectorBoolean(size_t n, bool x) : Vector() {

    for (size_t i = 0; i < n; i++)
        value.push_back(x);
}


/** Constructor from int vector */
VectorBoolean::VectorBoolean(const std::vector<bool>& x) : Vector() {

    value = x;
}


/** Constructor from int vector */
VectorBoolean::VectorBoolean(const std::vector<int>& x) : Vector() {

    for (std::vector<int>::const_iterator i=x.begin(); i!=x.end(); i++)
        value.push_back( (bool)(*i) );
}


/** Constructor from container iterator */
VectorBoolean::VectorBoolean(const ContainerIterator& x) : Vector() {

    for (ContainerIterator::const_iterator i=x.begin(); i!=x.end(); i++)
        value.push_back(*i);
}


/** Equals comparison */
bool VectorBoolean::operator==(const VectorBoolean& x) const {

    if (size() != x.size())
        return false;

    for (size_t i=0; i<value.size(); i++) {
        if (value[i] != x.value[i])
            return false;
    }

    return true;
}


/** Not equals comparison */
bool VectorBoolean::operator!=(const VectorBoolean& x) const {

    return !operator==(x);
}


/** Clone function */
VectorBoolean* VectorBoolean::clone() const {

    return new VectorBoolean(*this);
}


/** Convert to object of another class. The caller manages the object. */
RbObject* VectorBoolean::convertTo(const std::string& type) const {

    return NULL;
}


/** Pointer-based equals comparison */
bool VectorBoolean::equals(const RbObject* obj) const {

    // Use built-in fast down-casting first
    const VectorBoolean* p = dynamic_cast<const VectorBoolean*> (obj);
    if (p != NULL) {
        if (value.size() != p->value.size())
            return false;
        for (size_t i = 0; i < value.size(); i++) {
            if (value[i] != p->value[i])
                return false;
        }
        return true;
    }

    // Try converting the value to an int vector
    p = dynamic_cast<const VectorBoolean*> (obj->convert(getType()));
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
const VectorString& VectorBoolean::getClass() const {

    static VectorString rbClass = VectorString(VectorBoolean_name) + Vector::getClass();
    return rbClass;
}


/** Get element for parser (read-only) */
const RbObject* VectorBoolean::getElement(const VectorInteger& index) const {

    static Boolean x = Boolean(0);

    if (index.size() != 1)
        throw (RbException("Index error"));
    if (index[0] >= (int)value.size() || index[0] < 0)
        throw (RbException("Index out of bound"));

    x.setValue(value[index[0]]);
    return &x;
}


/** Get element class */
const std::string& VectorBoolean::getElementType(void) const {

    static std::string rbType = Boolean_name;
    return rbType;
}


/** Get element length for parser */
const VectorInteger& VectorBoolean::getLength(void) const {

    static VectorInteger length = VectorInteger(0);

    length[0] = int(value.size());
    return length;
}


/** Convert to object of another class. The caller manages the object. */
bool VectorBoolean::isConvertibleTo(const std::string& type) const {

    return false;
}


/** Print value for user */
void VectorBoolean::printValue(std::ostream& o) const {

    o << "[ ";
    for (std::vector<bool>::const_iterator i = value.begin(); i!= value.end(); i++) 
        {
        if (i != value.begin())
            o << ", ";
        o << (*i);
        }
    o <<  " ]";
    if (getIsRowVector() == false)
        o << "'";
}


/** Allow the parser to resize the vector */
void VectorBoolean::resize(const VectorInteger& len) {

    if (len.size() != 1 || len[0] < 0)
        throw (RbException("Length specification error"));

    value.resize(len[0]);
}


/** Allow parser to set an element (any type conversion is done by parser) */
void VectorBoolean::setElement(const VectorInteger& index, RbObject* val) {

    if ( !val->isType(Boolean_name) )
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

    value[index[0]] = ((Integer*)(val))->getValue();
}


/** Allow parser to rearrange the container (actually do not allow it) */
void VectorBoolean::setLength(const VectorInteger& len) {

    if ( len.size() != 1 && len[0] != int(value.size()) )
        throw (RbException("Length specification error"));
}


/** Set value of vector using VectorInteger */
void VectorBoolean::setValue(const VectorBoolean& x) {

    value.resize(x.size());
    for (size_t i=0; i<x.size(); i++)    
        value[i] = x.value[i];
}   


/** Complete info about object */
std::string VectorBoolean::toString(void) const {

    std::ostringstream o;
    o <<  "VectorBoolean: value = ";
    printValue(o);

    return o.str();
}

