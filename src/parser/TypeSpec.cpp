/**
 * @file
 * This file contains the implementation of TypeSpec, which
 * is used to hold type specifications.
 *
 * @brief Implementation of TypeSpec
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2010-09-06, version 1.0
 *
 * $Id$
 */

#include "TypeSpec.h"


/** Constructor for single object (dim=0)*/
TypeSpec::TypeSpec(const std::string& objType) : type(objType), dim(0), reference(ref) {
}


/** Complete constructor, also accommodating fields (plate/container) of objects */
TypeSpec::TypeSpec(const std::string& objType, int objDim, bool ref) : type(objType), dim(objDim), reference(ref) {
}


/** Equals comparison */
bool TypeSpec::operator==(const TypeSpec& x) const {

    if (type == x.type && dim == x.dim && reference == x.reference)
        return true;
    else
        return false;
}


/** Convert to string */
std::string TypeSpec::richInfo(void) const {

    std::string typeDesc = type;

    for (int i=0; i<dim; i++)
        typeDesc += "[]";

    if (reference)
        typeDesc += "&";

    return typeDesc;
}


/** Make sure we can print to stream using << operator */
std::ostream& operator<<(std::ostream& o, const TypeSpec& x) {

    o << x.richInfo();
    return o;
}

