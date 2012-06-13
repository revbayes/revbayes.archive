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
RlContinuousCharacterState::RlContinuousCharacterState(void) : RlCharacterState() {

}


/** Copy constructor */
RlContinuousCharacterState::RlContinuousCharacterState(const RlContinuousCharacterState& s) : RlCharacterState(), state( s.state ) {

}


/** Equals comparison */
bool RlContinuousCharacterState::operator==(const RlCharacterState& x) const {

    const RlContinuousCharacterState* derivedX = dynamic_cast<const RlContinuousCharacterState*>(&x);

    if ( derivedX != NULL )
        return state == derivedX->state;
    return false;
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
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RlCharacterState::getClassTypeSpec() ) );
    
	return rbClass; 
}


RbValue<void*> RlContinuousCharacterState::getLeanValue( void ) const {
    RbValue<void*> tmp;
    tmp.value = static_cast<void*>( &const_cast<ContinuousCharacterState &>( state ) );
    return tmp;
}

/** Get type spec */
const TypeSpec& RlContinuousCharacterState::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


const ContinuousCharacterState& RlContinuousCharacterState::getValue( void ) const {
    return state;
}


size_t RlContinuousCharacterState::memorySize() const {
    return sizeof( ContinuousCharacterState );
}


/** Print information for the user */
void RlContinuousCharacterState::printValue(std::ostream &o) const {

    o << state.getStringValue();
}


