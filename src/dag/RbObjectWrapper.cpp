/**
 * @file
 * This file contains the implementation of some of the functions
 * in the abstract base class for wrappers of language objects,
 * RbObjectWrapper.
 *
 * @brief Partial implementation of RbObjectWrapper
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-02, version 1.0
 * @extends RbObjectWrapper
 *
 * $Id$
 */

#include "RbException.h"
#include "RbNames.h"
#include "RbObjectWrapper.h"
#include "StringVector.h"

#include <sstream>


/** Get class vector describing type of object */
const StringVector& RbObjectWrapper::getClass(void) const { 

    static StringVector rbClass = StringVector(RbNames::RbObjectWrapper::name);
	return rbClass; 
}


/** Get element variable; default throws error, override if wrapper has variable elements */
const RbObjectWrapper* RbObjectWrapper::getVarElement(const IntVector& index) const {

    throw (RbException("No variable elements"));
}


/** Are we of specified type? We need to check entire class vector in case we are derived from type. */
bool RbObjectWrapper::isType(const std::string& type) const {

    const StringVector& classVec = getClass();

    for (size_t i=0; i<classVec.size(); i++) {
        if (type == classVec[i])
            return true;
    }

	return false;
}


/** Print struct for user; only wrapperClass here, add more in derived classes */
void RbObjectWrapper::printStruct(std::ostream& o) const {

    o << "$wrapperClass = ";
    getClass().printValue(o);
    o << std::endl;
}


/** Set element variable; default throws error, override if wrapper has variable elements */
void RbObjectWrapper::setElement(const IntVector& index, RbObjectWrapper* var) {

    throw (RbException("No variable elements"));
}


