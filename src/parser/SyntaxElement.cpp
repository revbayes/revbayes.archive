/**
 * @file
 * This file contains the implementation of some of the functions
 * in the abstract base class for syntax elements.
 *
 * @brief Partial implementation of SyntaxElement
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-02, version 1.0
 *
 * $Id$
 */

#include "RbNames.h"
#include "SyntaxElement.h"
#include "StringVector.h"


/** Get class vector describing type of object */
const StringVector& SyntaxElement::getClass(void) const { 

    static StringVector rbClass = StringVector(SyntaxElement_name);
	return rbClass; 
}


/** Get type (first entry in class vector) */
const std::string& SyntaxElement::getType(void) const {

    return getClass()[0];
}


/** Are we of specified type? We need to check entire class vector in case we are derived from type. */
bool SyntaxElement::isType(const std::string& type) const {

    const StringVector& classVec = getClass();

    for (size_t i=0; i<classVec.size(); i++) {
        if (type == classVec[i])
            return true;
    }

	return false;
}


