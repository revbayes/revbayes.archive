/**
 * @file
 * This file contains the implementation of VectorString, a complex type
 * used to hold string vectors.
 *
 * @brief Implementation of VectorString
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 * @extends RbComplex
 *
 * $Id$
 */

#include "VectorInteger.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbString.h"
#include "VectorString.h"

#include <sstream>


/** Construct empty vector of length n */
VectorString::VectorString(size_t n) : RbComplex() {

    value.resize(n);
}


/** Construct vector with one string x */
VectorString::VectorString(const std::string& x) : RbComplex() {

    value.push_back(x);
}


/** Construct vector with n strings x */
VectorString::VectorString(size_t n, const std::string& x) : RbComplex() {

    for (size_t i = 0; i < n; i++)
        value.push_back(x);
}


/** Constructor from std::string vector */
VectorString::VectorString(const std::vector<std::string>& x) : RbComplex() {

    value = x;
}


/** Comparison  with operator== */
bool VectorString::operator==(const VectorString& sv) const {

	if ( size() != sv.size() )
		return false;
	for (size_t i=0; i<size(); i++)
		{
		if (value[i] != sv.value[i])
			return false;
		}
	return true;
}


/** Concatenation with operator+ */
VectorString VectorString::operator+(const VectorString& x) const {

    VectorString tempVec = *this;
    for (size_t i = 0; i < x.value.size(); i++)
        tempVec.push_back(x.value[i]);

    return tempVec;
}


/** Clone function */
VectorString* VectorString::clone() const {

    return new VectorString(*this);
}


/** Pointer-based equals comparison */
bool VectorString::equals(const RbObject* obj) const {

    // Use built-in fast down-casting first
    const VectorString* p = dynamic_cast<const VectorString*> (obj);
    if (p != NULL) {
        if (value.size() != p->value.size())
            return false;
        for (size_t i=0; i<value.size(); i++) {
            if (value[i] != p->value[i])
                return false;
        }
        return true;
    }

    // Try converting the value to a string vector
    p = dynamic_cast<const VectorString*> (obj->convertTo(getType()));
    if (p == NULL)
        return false;

    bool result = true;
    if (value.size() != p->value.size())
        result = false;
    else {
        for (size_t i=0; i<value.size(); i++)
            result = result && (value[i] == p->value[i]);
    }

    delete p;
    return result;
}


/** Get class vector describing type of object */
const VectorString& VectorString::getClass() const {

    static VectorString rbClass = VectorString(VectorString_name) + RbComplex::getClass();
    return rbClass;
}


/** Get element for parser (read-only) */
const RbObject* VectorString::getElement(const VectorInteger& index) const {

    static RbString x;

    if (index.size() != 0)
        throw (RbException("Index error"));
    if (index[0] >= (int)value.size() || index[0] < 0)
        throw (RbException("Index out of bound"));

    x.setValue(value[index[0]]);
    return &x;
}


/** Get element class */
const std::string& VectorString::getElementType(void) const {

    static std::string rbType = RbString_name;
    return rbType;
}


/** Get element length for parser */
const VectorInteger& VectorString::getLength() const {

    static VectorInteger length = VectorInteger(0);

    length[0] = int(value.size());
    return length;
}


/** Allow parser to resize the string vector */
void VectorString::resize(VectorInteger const& len) {

    if ( len.size() != 1 )
        throw (RbException("Length specification error"));

    value.resize(len[0]);
}


/** Allow parser to set an element (any type conversion is done by the parser) */
void VectorString::setElement(const VectorInteger& index, RbObject* val) {

    if ( !val->isType(RbString_name) )
        throw (RbException("Type mismatch"));

    if (index.size() != 1 || index[0] < 1 )
        throw (RbException("Index error"));

    if ( index[0] >= int(value.size()) ) {
        int oldLen = int(value.size());
        resize(index[0]);
        for (int i=oldLen; i<index[0]; i++)
            value[i] = "";
    }

    value[index[0]] = ((RbString*)(val))->getValue();
}


/** Allow parser to rearrange the container (actually do not allow it) */
void VectorString::setLength(const VectorInteger& len) {

    if ( len.size() != 1 && len[0] != int(value.size()) )
        throw (RbException("Length specification error"));
}


/** Print value for user */
void VectorString::printValue(std::ostream& o) const {

    o << "[ ";
    for (std::vector<std::string>::const_iterator i = value.begin(); i!= value.end(); i++) {
        if (i != value.begin())
            o << ", ";
        o << "\"" << (*i) << "\"";
    }
    o <<  " ]";
}


/** Complete info about object */
std::string VectorString::richInfo(void) const {

    std::ostringstream o;
    o << "VectorString: value = ";
    printValue(o);
    
    return o.str();
}

