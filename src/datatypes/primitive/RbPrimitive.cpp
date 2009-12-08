/**
 * @file
 * This file contains the implementation of RbPrimitive, which is
 * a collective holder for primitive types in the RevBayes language.
 *
 * @brief Implementation of RbPrimitive
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 * @extends RbObject
 *
 * $Id$
 */


#include "RbNames.h"
#include "RbPrimitive.h"
#include "StringVector.h"


/** Get class vector describing type of object */
const StringVector& RbPrimitive::getClass() const {

    static StringVector rbClass = StringVector(RbNames::RbPrimitive::name) + RbObject::getClass();
    return rbClass;
}


