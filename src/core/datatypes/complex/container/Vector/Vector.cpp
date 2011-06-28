/**
 * @file
 * This file contains the declaration of Vector, a complex type
 * that acts as a base class for all vectors.
 *
 * @brief Declaration of Vector
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 * @extends RbComplex
 *
 * $Id:$
 */

#include "RbException.h"
#include "RbNames.h"
#include "TypeSpec.h"
#include "Vector.h"
#include "VectorString.h"


/** Set type of elements */
Vector::Vector(const std::string& elemType)
    : ValueContainer(TypeSpec(elemType, 1)) {
}


/** Get class vector describing type of object */
const VectorString& Vector::getClass(void) const { 

    static VectorString rbClass = VectorString(Vector_name) + Container::getClass();
	return rbClass;
}


/** Pop element off of front of vector, updating length in process */
void Vector::pop_front(void) {

    delete elements.front();
    elements.erase(elements.begin());
    length[0]--;
}


/** Pop element off of back of vector, updating length in process */
void Vector::pop_back(void) {

    delete elements.back();
    elements.pop_back();
    length[0]--;
}


/** Resize vector */
void Vector::resize( size_t n ) {

    if ( n < elements.size() )
        throw RbException( "Invalid attempt to shrink vector" );

    for ( size_t i = elements.size(); i < n; i++ )
        elements.push_back( getDefaultElement() );
    length[0] = elements.size();
}

