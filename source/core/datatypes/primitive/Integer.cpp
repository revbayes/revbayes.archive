/**
 * @file
 * This file contains the implementation of Integer, which is
 * a RevBayes wrapper around a regular int.
 *
 * @brief Implementation of Integer
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 * @extends RbObject
 *
 * $Id$
 */


#include "Boolean.h"
#include "Integer.h"
#include "RealPos.h"
#include "Real.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbString.h"
#include "TypeSpec.h"
#include "VectorInteger.h"
#include "VectorString.h"

#include <sstream>

/** Default constructor */
Integer::Integer(void) : RbObject(), value(0) {
}

/** Construct from int */
Integer::Integer(const int v) : RbObject(), value(v) {
}

/** Construct from unsigned int (ambiguous between int and bool otherwise) */
Integer::Integer(const unsigned int v) : RbObject(), value(v) {
}

/** Construct from bool */
Integer::Integer(const bool v) : RbObject() {

    if (v) value = 1;
    else value = 0;
}

/** Clone object */
Integer* Integer::clone(void) const {

	return  new Integer(*this);
}


/** Convert to type and dim. The caller manages the returned object. */
RbObject* Integer::convertTo( const std::string& type, int dim ) const {

    if ( type == Boolean_name && dim == 0 ) 
        return new Boolean(value == 0);

    if ( type == Real_name && dim == 0 )
        return new Real(value);

    if ( type == RbString_name && dim == 0 ) {
        std::ostringstream o;
        printValue(o);
        return new RbString(o.str());
    }

    if ( type == VectorInteger_name && dim == 0 )
        return new VectorInteger(value);

    if ( type == RealPos_name && dim == 0 && value > 0 )
        return new RealPos(value);

    return RbObject::convertTo( type, dim );
}


/** Get class vector describing type of object */
const VectorString& Integer::getClass() const {

    static VectorString rbClass = VectorString(Integer_name) + RbObject::getClass();
    return rbClass;
}


/** Is convertible to language object of type and dim? */
bool Integer::isConvertibleTo(const std::string& type, int dim, bool once) const {

    if (type == Boolean_name && dim == 0)
        return true;

    if (type == Real_name && dim == 0)
        return true;

    if (type == RbString_name && dim == 0)
        return true;
    
    if (type == VectorInteger_name && dim == 0)
        return true;

    if (type == RealPos_name && dim == 0 && once == true && value > 0)
        return true;

    return RbObject::isConvertibleTo(type, dim, once);
}


/** Print value for user */
void Integer::printValue(std::ostream &o) const {

    o << value;
}

/** Get complete info about object */
std::string Integer::richInfo(void) const {

	std::ostringstream o;
    o << "Integer(";
    printValue(o);
	o<< ")";

    return o.str();
}

