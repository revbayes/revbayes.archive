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
 * $Id$
 */

#include "NucleotideState.h"
#include "RbNames.h"
#include "VectorString.h"



/** Default constructor */
NucleotideState::NucleotideState(void) : CharacterObservationDiscrete( 4 ) {

}


/** Get class vector describing type of object */
const VectorString& NucleotideState::getClass() const {

    static VectorString rbClass = VectorString( NucleotideState_name ) + RbObject::getClass();
    return rbClass;
}


/** Set the value */
void NucleotideState::setValue(const char s) {

    char c = tolower(s);
    if (c == 'u')
        c = 't';
        
	if (s == 'a')
		{
		value[0] = true;
		value[1] = false;
		value[2] = false;
		value[3] = false;
		}
	else if (s == 'c')
		{
		value[0] = false;
		value[1] = true;
		value[2] = false;
		value[3] = false;
		}
	else if (s == 'm')
		{
		value[0] = true;
		value[1] = true;
		value[2] = false;
		value[3] = false;
		}
	else if (s == 'g')
		{
		value[0] = false;
		value[1] = false;
		value[2] = true;
		value[3] = false;
		}
	else if (s == 'r')
		{
		value[0] = true;
		value[1] = false;
		value[2] = true;
		value[3] = false;
		}
	else if (s == 's')
		{
		value[0] = false;
		value[1] = true;
		value[2] = true;
		value[3] = false;
		}
	else if (s == 'v')
		{
		value[0] = true;
		value[1] = true;
		value[2] = true;
		value[3] = false;
		}
	else if (s == 't')
		{
		value[0] = false;
		value[1] = false;
		value[2] = false;
		value[3] = true;
		}
	else if (s == 'w')
		{
		value[0] = true;
		value[1] = false;
		value[2] = false;
		value[3] = true;
		}
	else if (s == 'y')
		{
		value[0] = false;
		value[1] = true;
		value[2] = false;
		value[3] = true;
		}
	else if (s == 'h')
		{
		value[0] = true;
		value[1] = true;
		value[2] = false;
		value[3] = true;
		}
	else if (s == 'k')
		{
		value[0] = false;
		value[1] = false;
		value[2] = true;
		value[3] = true;
		}
	else if (s == 'd')
		{
		value[0] = true;
		value[1] = false;
		value[2] = true;
		value[3] = true;
		}
	else if (s == 'b')
		{
		value[0] = false;
		value[1] = true;
		value[2] = true;
		value[3] = true;
		}
	else if (s == 'n')
		{
		value[0] = true;
		value[1] = true;
		value[2] = true;
		value[3] = true;
		}
}

