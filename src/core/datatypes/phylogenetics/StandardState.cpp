/**
 * @file
 * This file contains the implementation of StandardState, which is
 * the base class for the Standard character data type in RevBayes.
 *
 * @brief Implementation of StandardState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include "RbNames.h"
#include "StandardState.h"
#include "VectorString.h"
#include <sstream>

const std::string StandardState::stateLabels = "0123456789";



/** Default constructor */
StandardState::StandardState(void) : CharacterObservationDiscrete(10) {

    setValue('n');
}


/** Copy constructor */
StandardState::StandardState(const StandardState& s) : CharacterObservationDiscrete(10) {

    value = s.value;
}


/** Constructor that sets the observation */
StandardState::StandardState(const char s) : CharacterObservationDiscrete(10) {

    setValue(s);
}


/** Clone object */
StandardState* StandardState::clone(void) const {

	return new StandardState( *this );
}


/** Get class vector describing type of object */
const VectorString& StandardState::getClass(void) const {

    static VectorString rbClass = VectorString( StandardState_name ) + RbObject::getClass();
    return rbClass;
}


/** Get value */
const char StandardState::getValue(void) const {

    char c;
    size_t numMatches = 0;
    for (size_t i=0; i<numStates; i++)
        {
        if ( value[i] == true )
            {
            c = stateLabels[i];
            numMatches++;
            }
        }
    if (numMatches > 1)
        c = '?';
    return c;
}


/** Print information for the user */
void StandardState::printValue(std::ostream &o) const {

    o << getValue();
}


/** Get complete info about object */
std::string StandardState::richInfo( void ) const {

	std::ostringstream o;
    o << "Standard(";
    printValue( o );
	o << ")";

    return o.str();
}


/** Set the observation */
void StandardState::setValue(const char s) {

    // wipe the value clean, setting all bool flags to false
    for (size_t i=0; i<numStates; i++)
        value[i] = false;
        
    // look for matches against the state label static vector
    char c = toupper(s);
    size_t numMatches = 0;
    for (size_t i=0; i<numStates; i++)
        {
        if ( c == stateLabels[i] )
            {
            value[i] = true;
            numMatches++;
            }
        }
        
    // if there are no matches, we assume that the character state was something like
    // a '?', 'n', or '-' and set all of the flags to true, to indicate complete ambiguity
    if (numMatches == 0)
        {
        for (size_t i=0; i<numStates; i++)
            value[i] = true;
        }
}