/**
 * @file
 * This file contains the implementation of some of the functions
 * in the abstract base class for internal language objects, RbInternal.
 *
 * @brief Partial implementation of RbInternal
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-02, version 1.0
 * @extends RbInternal
 *
 * $Id$
 */

#include "RbNames.h"
#include "RbInternal.h"
#include "StringVector.h"

#include <sstream>


/** Get class vector describing type of object */
const StringVector& RbInternal::getClass(void) const { 

    static StringVector rbClass = StringVector(RbInternal_name);
	return rbClass; 
}


/** Get type (first entry in class vector) */
const std::string& RbInternal::getType(void) const {

    return getClass()[0];
}


/** Are we of specified type? We need to check entire class vector in case we are derived from type. */
bool RbInternal::isType(const std::string& type) const {

    const StringVector& classVec = getClass();

    for (size_t i=0; i<classVec.size(); i++) {
        if (type == classVec[i])
            return true;
    }

	return false;
}


/** Print info about object simply by using toString function */
void RbInternal::print(std::ostream& o) const {

    o << toString() << std::endl;
}

