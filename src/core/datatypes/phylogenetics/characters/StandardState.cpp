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



/** Default constructor */
StandardState::StandardState(void) : CharacterStateDiscrete(1) {

}


/** Copy constructor */
StandardState::StandardState(const StandardState& s) : CharacterStateDiscrete(s.numStates) {

    value = s.value;
}


StandardState::StandardState(const std::string ss) : CharacterStateDiscrete(ss.size()) {

    setNumberOfStates(ss.size());
    setStateLabels(ss);
}


/** Equals comparison */
bool StandardState::operator==(const Character& x) const {

    const StandardState* derivedX = static_cast<const StandardState*>(&x);
    for (size_t i=0; i<numStates; i++) 
        {
        if ( value[i] != derivedX->value[i] )
            return false;
        }
    return true;
}


/** Not equals comparison */
bool StandardState::operator!=(const Character& x) const {

    return !operator==(x);
}


/** Clone object */
StandardState* StandardState::clone(void) const {

	return new StandardState( *this );
}


/** Set the observation */
void StandardState::addState(const char s) {
        
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


/** Get class vector describing type of object */
const VectorString& StandardState::getClass(void) const {

    static VectorString rbClass = VectorString( StandardState_name ) + CharacterStateDiscrete::getClass();
    return rbClass;
}


/** Get value */
const char StandardState::getState(void) const {

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

    if ( getNumberOfStates() > 1 )
        {
        o << "(";
        for (size_t i=0; i<numStates; i++)
            {
            if (value[i] == true)
                o << stateLabels[i];
            }
        o << ")";
        }
    else
        o << getState();
}


/** Get complete info about object */
std::string StandardState::richInfo( void ) const {

	std::ostringstream o;
    printValue( o );
    return o.str();
}


/** Set the observation */
void StandardState::setState(const char s) {

    // wipe the value clean, setting all bool flags to false
    for (size_t i=0; i<numStates; i++)
        value[i] = false;
    addState(s);
}