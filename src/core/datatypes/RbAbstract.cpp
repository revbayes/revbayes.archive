/**
 * @file
 * This file contains the implementation of RbAbstract, which is
 * used to describe abstract types in the type table.
 *
 * @brief Implementation of RbAbstract
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */


#include "Boolean.h"
#include "RbAbstract.h"
#include "RealPos.h"
#include "Real.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbString.h"
#include "TypeSpec.h"
#include "VectorInteger.h"
#include "VectorString.h"

#include <sstream>

/** Constructor */
RbAbstract::RbAbstract( const VectorString classVec, size_t numDims, const std::string& elemType )
: RbObject(), classVector( classVec ), dim( numDims ), elementType( elemType ) {
}


/** Clone object */
RbAbstract* RbAbstract::clone( void ) const {

	return  new RbAbstract( *this );
}


/** Convert to type and dim: no conversion possible. */
RbObject* RbAbstract::convertTo( const std::string& type, size_t dim ) const {

    return RbObject::convertTo( type, dim );
}


/** Get language type specification of abstract class */
const TypeSpec RbAbstract::getTypeSpec( void ) const {

    if ( dim > 0 )
        return TypeSpec( elementType, dim );
    else
        return TypeSpec( getType(), 0 );
}


/** Is convertible to language object of type and dim? */
bool RbAbstract::isConvertibleTo(const std::string& type, size_t dim, bool once) const {

    return RbObject::isConvertibleTo(type, dim, once);
}


/** Print value for user */
void RbAbstract::printValue(std::ostream &o) const {

    o << classVector;
}

/** Get complete info about object */
std::string RbAbstract::richInfo(void) const {

	std::ostringstream o;
    o << "RbAbstract(";
    printValue(o);
	o<< ")";

    return o.str();
}

