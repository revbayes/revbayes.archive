/**
 * @file
 * This file contains the implementation of RlContinuousCharacterState, which is
 * the base class for the continuous character data type in RevBayes.
 *
 * @brief Implementation of RlContinuousCharacterState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include "RbUtil.h"
#include "RlContinuousCharacterState.h"
#include <cmath>
#include <sstream>


/** Default constructor */
RlContinuousCharacterState::RlContinuousCharacterState(void) : Character() {

    mean     = 0.0;
    variance = 0.0;
}


/** Copy constructor */
RlContinuousCharacterState::RlContinuousCharacterState(const RlContinuousCharacterState& s) : Character() {

    mean     = s.mean;
    variance = s.variance;
}


/** Constructor that sets the mean value */
RlContinuousCharacterState::RlContinuousCharacterState(const double x) : Character() {

    mean     = x;
    variance = 0.0;
}


/** Constructor that sets the mean and variance */
RlContinuousCharacterState::RlContinuousCharacterState(const double x, const double v) : Character() {

    mean     = x;
    variance = v;
}


/** Equals comparison */
bool RlContinuousCharacterState::operator==(const Character& x) const {

    const RlContinuousCharacterState* derivedX = static_cast<const RlContinuousCharacterState*>(&x);

    if ( fabs(mean - derivedX->mean) < 0.000000001 && fabs(variance - derivedX->variance) < 0.000000001 )
        return true;
    return false;
}


/** Not equals comparison */
bool RlContinuousCharacterState::operator!=(const Character& x) const {

    return !operator==(x);
}


/** Clone object */
RlContinuousCharacterState* RlContinuousCharacterState::clone(void) const {

	return new RlContinuousCharacterState( *this );
}


/** Get class name of object */
const std::string& RlContinuousCharacterState::getClassName(void) { 
    
    static std::string rbClassName = "Continuous character";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& RlContinuousCharacterState::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Character::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& RlContinuousCharacterState::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Print information for the user */
void RlContinuousCharacterState::printValue(std::ostream &o) const {

    if ( fabs(variance - 0.0) < 0.00000001 )
        o << mean;
    else
        o << mean << " (" << variance << ")";
}


