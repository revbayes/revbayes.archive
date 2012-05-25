/**
 * @file
 * This file contains the implementation of RlStandardState, which is
 * the base class for the Standard character data type in RevBayes.
 *
 * @brief Implementation of RlStandardState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include "RbUtil.h"
#include "RlStandardState.h"
#include <sstream>



/** Default constructor */
RlStandardState::RlStandardState(void) : RlDiscreteCharacterState() {

}


/** Copy constructor */
RlStandardState::RlStandardState(const StandardState& s) : RlDiscreteCharacterState() {

    state = s;
}


/** Equals comparison */
bool RlStandardState::operator==(const RlCharacterState& x) const {

    const RlStandardState* derivedX = dynamic_cast<const RlStandardState*>(&x);
    
    if ( derivedX != NULL ) {
        return state == derivedX->state;
    }
    
    return false;
}


/** Clone object */
RlStandardState* RlStandardState::clone(void) const {

	return new RlStandardState( *this );
}



/** Get class name of object */
const std::string& RlStandardState::getClassName(void) { 
    
    static std::string rbClassName = "Standard state";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& RlStandardState::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RlDiscreteCharacterState::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& RlStandardState::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


const StandardState& RlStandardState::getValue( void ) const {
    
    return state;
}


/** Print information for the user */
void RlStandardState::printValue(std::ostream &o) const {

    o << state.getStringValue();
}
