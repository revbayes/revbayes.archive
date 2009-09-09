/**
 * @file
 * This file contains the implementation of some functions in RbAbstractDataType,
 *  which is the REvBayes base class for the data types handled by the parser.
 *
 * @brief Implementation of RbAbstractDataType (some functions)
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2009-08-23 15:04:35 +0200 (Sön, 23 Aug 2009) $
 * @author The REvBayes development core team
 * @license GPL version 3
 *
 * $Id: RbDataType.h 9 2009-08-23 13:04:35Z ronquist $
 */

#include "RbAbstractDataType.h"
#include "RbInt.h"
#include "RbString.h"
#include <iostream>
#include <string>


/** Define the abstract type */
const string RbAbstractDataType::dataType = std::string("");


/** Constructor from type name (C style) */
RbAbstractDataType::RbAbstractDataType(const char* name)
    : dim(NULL), names(NULL), typeName(name) {
}

/** Constructor from type name (C++ stye) */
RbAbstractDataType::RbAbstractDataType(const string& name)
    : dim(NULL), names(NULL), typeName(name) {
}

/** Destructor frees names and dim attributes */
RbAbstractDataType::~RbAbstractDataType() {

    delete names;
    delete dim;
}

/** Set names attribute */
void RbAbstractDataType::setNames(const RbString &newNames) {

    if ( newNames.getSize() == getSize() ) {
        delete names;
        names = newNames.copy();
    }
}

/** Set dim attribute */
void RbAbstractDataType::setDim(const RbInt &newDim) {

    int dimSize = 1;
    for (RbInt::const_iterator i=newDim.begin(); i!=newDim.end(); i++)
        dimSize *= (*i);

    if ( dimSize == getSize() ) {
        delete dim;
        dim = newDim.copy();
    }
}
