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


/** Constructor for single element (dim=0)*/
TypeSpec::TypeSpec(const std::string& elemType, bool ref) : type(elemType), dim(0), reference(ref) {
}


/** Complete constructor */
TypeSpec::TypeSpec(const std::string& elemType, int elemDim, bool ref) : type(elemType), dim(elemDim), reference(ref) {
}


/** Equals comparison */
bool TypeSpec::operator==(const TypeSpec& x) const {

    if (type == x.type && dim == x.dim && reference == x.reference)
        return true;
    else
        return false;
}


/** Convert to string */
std::string TypeSpec::toString(void) const {

    std::string typeDesc = type;

    for (int i=0; i<dim; i++)
        typeDesc += "[]";

    if (reference)
        typeDesc += "&";

    return typeDesc;
}

