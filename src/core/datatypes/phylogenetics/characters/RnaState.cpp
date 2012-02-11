/**
 * @file
 * This file contains the implementation of RnaState, which is
 * the base class for the DNA character data type in RevBayes.
 *
 * @brief Implementation of RnaState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include "RbUtil.h"
#include "RnaState.h"
#include "VectorString.h"
#include <sstream>

const std::string RnaState::stateLabels = "ACGU";

// Definition of the static type spec member
const TypeSpec RnaState::typeSpec(RnaState_name);


/** Default constructor */
RnaState::RnaState(void) : NucleotideState() {

    setState('n');
}


/** Copy constructor */
RnaState::RnaState(const RnaState& s) : NucleotideState() {

    value = s.value;
}


/** Constructor that sets the observation */
RnaState::RnaState(const char s) : NucleotideState() {

    setState(s);
}


/** Constructor that sets the observation from a set of states */
RnaState::RnaState(const std::set<char> s) : NucleotideState() {

    char c = getNucleotideCode(s);
    setState(c);
}


/** Equals comparison */
bool RnaState::operator==(const Character& x) const {

    const RnaState* derivedX = static_cast<const RnaState*>(&x);
    for (size_t i=0; i<numStates; i++) 
        {
        if ( value[i] != derivedX->value[i] )
            return false;
        }
    return true;
}


/** Not equals comparison */
bool RnaState::operator!=(const Character& x) const {

    return !operator==(x);
}


/** Clone object */
RnaState* RnaState::clone(void) const {

	return  new RnaState( *this );
}


/** Get class vector describing type of object */
const VectorString& RnaState::getClass(void) const {

    static VectorString rbClass = VectorString( RnaState_name ) + NucleotideState::getClass();
    return rbClass;
}


const char RnaState::getNucleotideCode(const std::set<char>& s) const {

    std::vector<bool> stateSet(4);
    for (size_t i=0; i<4; i++)
        stateSet[i] = false;
    for (std::set<char>::const_iterator p = s.begin(); p != s.end(); p++)
        {
        char c = toupper(*p);
        if (c == 'A')
            stateSet[0] = true;
        else if (c == 'C')
            stateSet[1] = true;
        else if (c == 'G')
            stateSet[2] = true;
        else if (c == 'T' || c == 'U')
            stateSet[3] = true;
        }
    return getNucleotideCode(stateSet);
}


const char RnaState::getNucleotideCode(const std::vector<bool>& sSet) const {

	if (sSet[0] == true && sSet[1] == false && sSet[2] == false && sSet[3] == false)
		return 'A';
	else if (sSet[0] == false && sSet[1] == true  && sSet[2] == false && sSet[3] == false)
		return 'C';
	else if (sSet[0] == true  && sSet[1] == true  && sSet[2] == false && sSet[3] == false)
		return 'M';
	else if (sSet[0] == false && sSet[1] == false && sSet[2] == true  && sSet[3] == false)
		return 'G';
	else if (sSet[0] == true  && sSet[1] == false && sSet[2] == true  && sSet[3] == false)
		return 'R';
	else if (sSet[0] == false && sSet[1] == true  && sSet[2] == true  && sSet[3] == false)
		return 'S';
	else if (sSet[0] == true  && sSet[1] == true  && sSet[2] == true  && sSet[3] == false)
		return 'V'; 
	else if (sSet[0] == false && sSet[1] == false && sSet[2] == false && sSet[3] == true)
		return 'U';
	else if (sSet[0] == true  && sSet[1] == false && sSet[2] == false && sSet[3] == true)
		return 'W';
	else if (sSet[0] == false && sSet[1] == true  && sSet[2] == false && sSet[3] == true)
		return 'Y';
	else if (sSet[0] == true  && sSet[1] == true  && sSet[2] == false && sSet[3] == true)
		return 'H';
	else if (sSet[0] == false && sSet[1] == false && sSet[2] == true  && sSet[3] == true)
		return 'K';
	else if (sSet[0] == true  && sSet[1] == false && sSet[2] == true  && sSet[3] == true)
		return 'D';
	else if (sSet[0] == false && sSet[1] == true  && sSet[2] == true  && sSet[3] == true)
		return 'B';
	else if (sSet[0] == true  && sSet[1] == true  && sSet[2] == true  && sSet[3] == true)
		return 'N';
    return 'N';
}


/** Get value */
const char RnaState::getState(void) const {

    return getNucleotideCode(value);
}

std::string RnaState::getStringValue(void) const  {

    char c = getState();
    std::string s = "";
    s += c;
    return s;
}

/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& RnaState::getTypeSpec(void) const {
    return typeSpec;
}


/** Print information for the user */
void RnaState::printValue(std::ostream &o) const {

    o << getState();
}

