/**
 * @file
 * This file contains the implementation of VectorCharacterObservations, an
 * abstract base class for different types of character observation vectors.
 *
 * @brief Implementation of VectorCharacterObservations
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 * @extends RbComplex
 *
 * $Id$
 */

#include "RbException.h"
#include "RbNames.h"
#include "RbString.h"
#include "VectorCharacterObservations.h"



VectorCharacterObservations::VectorCharacterObservations(const std::string& elemType) : Vector(elemType) {

}


/** Get class vector describing type of object */
const VectorString& VectorCharacterObservations::getClass(void) const {

    static VectorString rbClass = VectorString(VectorCharacterObservations_name) + Vector::getClass();
    return rbClass;
}

