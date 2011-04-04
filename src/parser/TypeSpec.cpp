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

#include "RbException.h"
#include "RbNames.h"
#include "TypeSpec.h"
#include "Workspace.h"


/** Constructor for single object (dim = 0, ref = false; if container type, we translate it) */
TypeSpec::TypeSpec(const std::string& objType)
    : type(Workspace::userWorkspace().getTypeSpec(objType).getType()),
      dim (Workspace::userWorkspace().getTypeSpec(objType).getDim()),
      reference(false) {

    if ( Workspace::userWorkspace().areTypesInitialized() && Workspace::userWorkspace().isXOfTypeY( type, Container_name ) )
        throw RbException( "Cannot convert container type name to language type specification" );
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


/** Type conversion operator to std::string */
TypeSpec::operator std::string( void ) const {

    return toString();
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


/** Make sure we can print to stream using << operator */
std::ostream& operator<<(std::ostream& o, const TypeSpec& x) {

    o << x.toString();
    return o;
}


/** String concatenation using operator + */
std::string operator+(const std::string& s, const TypeSpec& x) {

    return s + x.toString();
}

