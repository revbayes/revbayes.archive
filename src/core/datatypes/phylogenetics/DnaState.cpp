/**
 * @file
 * This file contains the implementation of DnaState, which is
 * the base class for the DNA character data type in RevBayes.
 *
 * @brief Implementation of DnaState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include "DnaState.h"
#include "RbNames.h"
#include "VectorString.h"
#include <sstream>

const std::string DnaState::stateLabels = "ACGT";



/** Default constructor */
DnaState::DnaState(void) : NucleotideState() {

    setValue('n');
}


/** Copy constructor */
DnaState::DnaState(const DnaState& s) : NucleotideState() {

    value = s.value;
}


/** Constructor that sets the observation */
DnaState::DnaState(const char s) : NucleotideState() {

    setValue(s);
}


/** Clone object */
DnaState* DnaState::clone(void) const {

	return  new DnaState( *this );
}


/** Get class vector describing type of object */
const VectorString& DnaState::getClass(void) const {

    static VectorString rbClass = VectorString( DnaState_name ) + RbObject::getClass();
    return rbClass;
}


/** Get value */
const char DnaState::getValue(void) const {

	if (value[0] == true && value[1] == false && value[2] == false && value[3] == false)
		return 'A';
	else if (value[0] == false && value[1] == true  && value[2] == false && value[3] == false)
		return 'C';
	else if (value[0] == true  && value[1] == true  && value[2] == false && value[3] == false)
		return 'M';
	else if (value[0] == false && value[1] == false && value[2] == true  && value[3] == false)
		return 'G';
	else if (value[0] == true  && value[1] == false && value[2] == true  && value[3] == false)
		return 'R';
	else if (value[0] == false && value[1] == true  && value[2] == true  && value[3] == false)
		return 'S';
	else if (value[0] == true  && value[1] == true  && value[2] == true  && value[3] == false)
		return 'V'; 
	else if (value[0] == false && value[1] == false && value[2] == false && value[3] == true)
		return 'T';
	else if (value[0] == true  && value[1] == false && value[2] == false && value[3] == true)
		return 'W';
	else if (value[0] == false && value[1] == true  && value[2] == false && value[3] == true)
		return 'Y';
	else if (value[0] == true  && value[1] == true  && value[2] == false && value[3] == true)
		return 'H';
	else if (value[0] == false && value[1] == false && value[2] == true  && value[3] == true)
		return 'K';
	else if (value[0] == true  && value[1] == false && value[2] == true  && value[3] == true)
		return 'D';
	else if (value[0] == false && value[1] == true  && value[2] == true  && value[3] == true)
		return 'B';
	else if (value[0] == true  && value[1] == true  && value[2] == true  && value[3] == true)
		return 'N';
	return '*';
}


/** Print information for the user */
void DnaState::printValue(std::ostream &o) const {

    o << getValue();
}


/** Get complete info about object */
std::string DnaState::richInfo( void ) const {

	std::ostringstream o;
    o << "DNA(";
    printValue( o );
	o << ")";

    return o.str();
}
