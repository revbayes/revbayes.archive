/**
 * @file
 * This file contains the implementation of RlDnaState, which is
 * the base class for the Standard character data type in RevBayes.
 *
 * @brief Implementation of RlDnaState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-05-24 09:58:04 +0200 (Thu, 24 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: RlDnaState.cpp 1568 2012-05-24 07:58:04Z hoehna $
 */

#include "RbUtil.h"
#include "RlDnaState.h"
#include <sstream>



/** Default constructor */
RlDnaState::RlDnaState(void) : RlDiscreteCharacterState() {
    
}


/** Copy constructor */
RlDnaState::RlDnaState(const DnaState& s) : RlDiscreteCharacterState() {
    
    state = s;
}


/** Equals comparison */
bool RlDnaState::operator==(const RlCharacterState& x) const {
    
    const RlDnaState* derivedX = dynamic_cast<const RlDnaState*>(&x);
    
    if ( derivedX != NULL ) {
        return state == derivedX->state;
    }
    
    return false;
}


/** Clone object */
RlDnaState* RlDnaState::clone(void) const {
    
	return new RlDnaState( *this );
}



/** Get class name of object */
const std::string& RlDnaState::getClassName(void) { 
    
    static std::string rbClassName = "DNA state";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& RlDnaState::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RlDiscreteCharacterState::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& RlDnaState::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


const DnaState& RlDnaState::getValue( void ) const {
    
    return state;
}


/** Initialize the discrete character state variable. We just set the value. */
void RlDnaState::initialize(const std::vector<RbObject*> &attributes) {
    
    // set the state
    state.setState( static_cast<RbString*>( attributes[0] )->getValue()[0] );
    //    numStates = static_cast<Natural*>( (RbLanguageObject*)(*attr)[0] )->getValue();
}



/** Print information for the user */
void RlDnaState::printValue(std::ostream &o) const {
    
    o << state.getStringValue();
}
