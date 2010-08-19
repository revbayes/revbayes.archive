/**
 * @file
 * This file contains the implementation of some functions in
 * Container, the abstract base class (interface) for containers,
 * such as vectors, matrices and arrays.
 *
 * @brief Implementation of Container
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-05, version 1.0
 *
 * $Id$
 */

#include "Container.h"
#include "RbException.h"
#include "RbNames.h"
#include "VectorString.h"

#include <iostream>
#include <sstream>


/** Constructor sets elementType and initializes length */
Container::Container(const std::string& elemType)
    : RbComplex(), elementType(elemType), length(VectorInteger()) {
}


/** Get class vector describing type of object */
const VectorString& Container::getClass(void) const {

    static VectorString rbClass = VectorString(Container_name) + RbComplex::getClass();
    return rbClass;
}


/** Get subcontainer */
Container* Container::getSubContainer(const VectorInteger& index) const {

    throw RbException ("Extracting subcontainer not supported");

    return NULL;    // just in case the compile complains about return value
}


/** Get offset to element or subcontainer; also check index */
size_t Container::getOffset(const VectorInteger& index) const {

    if (index.size() > length.size())
        throw (RbException("Too many indices"));
    for (size_t i=0; i<index.size(); i++) {
        if (index[i] < 0 || index[i] >= length[i])
            throw (RbException("Index out of range"));
    }

    size_t offset = 0;
    int numVals = 1;
    int i;
    for (i=int(length.size())-1; i>=int(index.size()); i--)
         numVals *= length[i];
    for (; i>=0; i--) {
        offset += (index[i] * numVals);
        numVals *= length[i];
    }
 
    return offset;
}


/** Get the size of the container */
size_t Container::size(void) const {

    size_t size = 0;

    for (size_t i=0; i<length.size(); i++)
        size += length[i];

    return size;
}

