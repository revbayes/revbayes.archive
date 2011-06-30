/**
 * @file
 * This file contains the implementation of NucleotideState, which is
 * the base class for nucleotide character data types in RevBayes.
 *
 * @brief Implementation of NucleotideState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id:$
 */

#include "NucleotideState.h"
#include "RbNames.h"
#include "VectorString.h"



/** Default constructor */
NucleotideState::NucleotideState(void) : CharacterObservationDiscrete( 4 ) {

}


/** Equals comparison */
bool NucleotideState::operator==(const NucleotideState& x) const {

    return true;
}


/** Not equals comparison */
bool NucleotideState::operator!=(const NucleotideState& x) const {

    return !operator==(x);
}


/** Set the value */
void NucleotideState::addState(const char s) {

    char c = tolower(s);
    if (c == 'u')
        c = 't';
        
	if (c == 'a')
		{
		value[0] = true;
		}
	else if (c == 'c')
		{
		value[1] = true;
		}
	else if (c == 'm')
		{
		value[0] = true;
		value[1] = true;
		}
	else if (c == 'g')
		{
		value[2] = true;
		}
	else if (c == 'r')
		{
		value[0] = true;
		value[2] = true;
		}
	else if (c == 's')
		{
		value[1] = true;
		value[2] = true;
		}
	else if (c == 'v')
		{
		value[0] = true;
		value[1] = true;
		value[2] = true;
		}
	else if (c == 't')
		{
		value[3] = true;
		}
	else if (c == 'w')
		{
		value[0] = true;
		value[3] = true;
		}
	else if (c == 'y')
		{
		value[1] = true;
		value[3] = true;
		}
	else if (c == 'h')
		{
		value[0] = true;
		value[1] = true;
		value[3] = true;
		}
	else if (c == 'k')
		{
		value[2] = true;
		value[3] = true;
		}
	else if (c == 'd')
		{
		value[0] = true;
		value[2] = true;
		value[3] = true;
		}
	else if (c == 'b')
		{
		value[1] = true;
		value[2] = true;
		value[3] = true;
		}
	else if (c == 'n')
		{
		value[0] = true;
		value[1] = true;
		value[2] = true;
		value[3] = true;
		}
}


/** Get class vector describing type of object */
const VectorString& NucleotideState::getClass() const {

    static VectorString rbClass = VectorString( NucleotideState_name ) + RbObject::getClass();
    return rbClass;
}


/** Set the value */
void NucleotideState::setState(const char s) {

    char c = tolower(s);
    if (c == 'u')
        c = 't';
        
	if (c == 'a')
		{
		value[0] = true;
		value[1] = false;
		value[2] = false;
		value[3] = false;
		}
	else if (c == 'c')
		{
		value[0] = false;
		value[1] = true;
		value[2] = false;
		value[3] = false;
		}
	else if (c == 'm')
		{
		value[0] = true;
		value[1] = true;
		value[2] = false;
		value[3] = false;
		}
	else if (c == 'g')
		{
		value[0] = false;
		value[1] = false;
		value[2] = true;
		value[3] = false;
		}
	else if (c == 'r')
		{
		value[0] = true;
		value[1] = false;
		value[2] = true;
		value[3] = false;
		}
	else if (c == 's')
		{
		value[0] = false;
		value[1] = true;
		value[2] = true;
		value[3] = false;
		}
	else if (c == 'v')
		{
		value[0] = true;
		value[1] = true;
		value[2] = true;
		value[3] = false;
		}
	else if (c == 't')
		{
		value[0] = false;
		value[1] = false;
		value[2] = false;
		value[3] = true;
		}
	else if (c == 'w')
		{
		value[0] = true;
		value[1] = false;
		value[2] = false;
		value[3] = true;
		}
	else if (c == 'y')
		{
		value[0] = false;
		value[1] = true;
		value[2] = false;
		value[3] = true;
		}
	else if (c == 'h')
		{
		value[0] = true;
		value[1] = true;
		value[2] = false;
		value[3] = true;
		}
	else if (c == 'k')
		{
		value[0] = false;
		value[1] = false;
		value[2] = true;
		value[3] = true;
		}
	else if (c == 'd')
		{
		value[0] = true;
		value[1] = false;
		value[2] = true;
		value[3] = true;
		}
	else if (c == 'b')
		{
		value[0] = false;
		value[1] = true;
		value[2] = true;
		value[3] = true;
		}
	else if (c == 'n')
		{
		value[0] = true;
		value[1] = true;
		value[2] = true;
		value[3] = true;
		}
}

