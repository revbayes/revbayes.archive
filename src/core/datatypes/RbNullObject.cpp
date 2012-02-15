/**
 * @file
 * This file contains the declaration of RbNullObject, which is
 * the RevBayes class used for NULL references.
 *
 * @brief Declaration of RbNullObject
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-02-14
 *
 * $Id:$
 */


#include "RbNullObject.h"
#include "VectorString.h"

#include <sstream>

RbNullObject::RbNullObject(void) : RbLanguageObject() {
    
}


RbNullObject::RbNullObject(const RbNullObject& n) : RbLanguageObject(n) {
    
}


RbNullObject* RbNullObject::clone(void) const {
    return new RbNullObject(*this);
}


bool RbNullObject::operator==(void *ptr) {
    return ptr == NULL;
}


/** Get class vector describing type of object */
const VectorString& RbNullObject::getClass(void) const { 
    
    static VectorString rbClass = VectorString(RbNullObject_name) + RbLanguageObject::getClass();
	return rbClass; 
}



/** Print the value. We just print "NULL" */
void RbNullObject::printValue(std::ostream &o) const {
    o << "NULL";
}



/** Make sure we can print the value of the object easily */
std::ostream& operator<<(std::ostream& o, const RbNullObject& x) {
    
    o << "NULL";
    return o;
}

