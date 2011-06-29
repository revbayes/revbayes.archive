/**
 * @file
 * This file contains the implementation of CharacterObservationDiscrete, which is
 * the abstract base class for discrete character data types in RevBayes.
 *
 * @brief Implementation of CharacterObservationDiscrete
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id:$
 */

#include "CharacterObservationDiscrete.h"
#include "RbNames.h"
#include "VectorString.h"



/** Constructor taking the number of states */
CharacterObservationDiscrete::CharacterObservationDiscrete(size_t n) : CharacterObservation(), numStates(n) {

    value.resize(numStates);
}


/** Get class vector describing type of object */
const VectorString& CharacterObservationDiscrete::getClass() const {

    static VectorString rbClass = VectorString( CharacterObservationDiscrete_name ) + RbObject::getClass();
    return rbClass;
}


size_t CharacterObservationDiscrete::getNumOnStates(void) const {

    size_t nOn = 0;
    for (size_t i=0; i<value.size(); i++)
        {
        if (value[i] == true)
            nOn++;
        }
    return nOn;
}


void CharacterObservationDiscrete::setNumStates(size_t n) {

    numStates = n;
    value.resize(n);
}


