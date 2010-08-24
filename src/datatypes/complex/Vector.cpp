/**
 * @file
 * This file contains the declaration of Vector, a complex type
 * that acts as a base class for all vectors.
 *
 * @brief Declaration of Vector
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 * @extends RbComplex
 *
 * $Id$
 */

#include "RbNames.h"
#include "Vector.h"
#include "VectorString.h"



/** Default constructor*/
Vector::Vector(void) {

    isRowVector = true;
}

/** Get class vector describing type of object */
const VectorString& Vector::getClass(void) const { 

    static VectorString rbClass = VectorString(Vector_name) + RbComplex::getClass();
	return rbClass;
}

void Vector::transpose(void) {
std::cout << "Transposing vector isRowVector = " << isRowVector << "->";
    if (isRowVector == true)
        isRowVector = false;
    else 
        isRowVector = true;
std::cout << isRowVector << std::endl;
}