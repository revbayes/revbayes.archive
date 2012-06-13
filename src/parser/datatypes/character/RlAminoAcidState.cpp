/**
 * @file
 * This file contains the implementation of AminoAcidState, which is
 * the class for the Amino Acid character data type in RevBayes.
 *
 * @brief Implementation of AminoAcidState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include "RlAminoAcidState.h"
#include "RbUtil.h"
#include <sstream>


/** Default constructor */
RlAminoAcidState::RlAminoAcidState(void) : RlDiscreteCharacterState() {

}


/** Copy constructor */
RlAminoAcidState::RlAminoAcidState(const RlAminoAcidState& s) : RlDiscreteCharacterState(), state( s.state ) {

}

/** Equals comparison */
bool RlAminoAcidState::operator==(const RlCharacterState& x) const {

    const RlAminoAcidState* derivedX = dynamic_cast<const RlAminoAcidState*>(&x);
    
    if ( derivedX != NULL ) {
        return state == derivedX->state;
    }
    
    return false;
}


/* Clone object */
RlAminoAcidState* RlAminoAcidState::clone(void) const {

	return new RlAminoAcidState( *this );
}


/** Get class name of object */
const std::string& RlAminoAcidState::getClassName(void) { 
    
    static std::string rbClassName = "Amino Acid";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& RlAminoAcidState::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RlDiscreteCharacterState::getClassTypeSpec() ) );
    
	return rbClass; 
}


RbValue<void*> RlAminoAcidState::getLeanValue( void ) const {
    RbValue<void*> tmp;
    tmp.value = static_cast<void*>( &const_cast<AminoAcidState&>( state ) );
    return tmp;
}

/* Get type spec */
const TypeSpec& RlAminoAcidState::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



const AminoAcidState& RlAminoAcidState::getValue( void ) const {
    return state;
}


size_t RlAminoAcidState::memorySize() const {
    return sizeof( AminoAcidState );
}



/** Print information for the user */
void RlAminoAcidState::printValue(std::ostream &o) const {

    o << state.getStringValue();
}
