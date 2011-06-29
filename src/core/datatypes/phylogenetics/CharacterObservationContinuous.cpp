/**
 * @file
 * This file contains the implementation of CharacterObservationContinuous, which is
 * the base class for the continuous character data type in RevBayes.
 *
 * @brief Implementation of CharacterObservationContinuous
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include "RbNames.h"
#include "CharacterObservationContinuous.h"
#include "VectorString.h"
#include <cmath>
#include <sstream>



/** Default constructor */
CharacterObservationContinuous::CharacterObservationContinuous(void) : CharacterObservation() {

    mean     = 0.0;
    variance = 0.0;
}


/** Copy constructor */
CharacterObservationContinuous::CharacterObservationContinuous(const CharacterObservationContinuous& s) : CharacterObservation() {

    mean     = s.mean;
    variance = s.variance;
}


/** Constructor that sets the mean value */
CharacterObservationContinuous::CharacterObservationContinuous(const double x) : CharacterObservation() {

    mean     = x;
    variance = 0.0;
}


/** Constructor that sets the mean and variance */
CharacterObservationContinuous::CharacterObservationContinuous(const double x, const double v) : CharacterObservation() {

    mean     = x;
    variance = v;
}


/** Equals comparison */
bool CharacterObservationContinuous::operator==(const CharacterObservationContinuous& x) const {

    if ( fabs(mean - x.mean) < 0.000000001 && fabs(variance - x.variance) < 0.000000001 )
        return true;
    return false;
}


/** Not equals comparison */
bool CharacterObservationContinuous::operator!=(const CharacterObservationContinuous& x) const {

    return !operator==(x);
}


/** Clone object */
CharacterObservationContinuous* CharacterObservationContinuous::clone(void) const {

	return new CharacterObservationContinuous( *this );
}


/** Get class vector describing type of object */
const VectorString& CharacterObservationContinuous::getClass(void) const {

    static VectorString rbClass = VectorString( CharacterObservationContinuous_name ) + RbObject::getClass();
    return rbClass;
}


/** Print information for the user */
void CharacterObservationContinuous::printValue(std::ostream &o) const {

    if ( fabs(variance - 0.0) < 0.00000001 )
        o << mean;
    else
        o << mean << " (" << variance << ")";
}


/** Get complete info about object */
std::string CharacterObservationContinuous::richInfo( void ) const {

	std::ostringstream o;
    printValue( o );

    return o.str();
}

