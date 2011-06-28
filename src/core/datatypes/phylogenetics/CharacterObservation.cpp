/**
 * @file
 * This file contains the implementation of CharacterObservation, which is
 * the abstract base class for CharacterObservation data types in RevBayes.
 *
 * @brief Implementation of CharacterObservation
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id:$
 */


#include "CharacterObservation.h"
#include "RbNames.h"
#include "VectorString.h"



/** Default constructor */
CharacterObservation::CharacterObservation(void) : RbObject() {

}


/** Get class vector describing type of object */
const VectorString& CharacterObservation::getClass() const {

    static VectorString rbClass = VectorString( CharacterObservation_name ) + RbObject::getClass();
    return rbClass;
}


