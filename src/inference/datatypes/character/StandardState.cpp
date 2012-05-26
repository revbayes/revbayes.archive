/**
 * @file
 * This file contains the implementation of StandardState, which is
 * the base class for the Standard character data type in RevBayes.
 *
 * @brief Implementation of StandardState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-05-24 09:58:04 +0200 (Thu, 24 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: StandardState.cpp 1568 2012-05-24 07:58:04Z hoehna $
 */

#include "StandardState.h"
#include "RbUtil.h"
#include <sstream>


/** Default constructor */
StandardState::StandardState(void) : DiscreteCharacterState(), state() {
    
}


/** Default constructor */
StandardState::StandardState(const std::string &s) : DiscreteCharacterState(), state() {
    
}


/** Copy constructor */
StandardState::StandardState(const StandardState& s) : DiscreteCharacterState(), state( s.state ) {
    
}


/** Equals comparison */
bool StandardState::operator==(const CharacterState& x) const {
    
    const StandardState* derivedX = dynamic_cast<const StandardState*>( &x );
    
    if (derivedX != NULL) {
        return derivedX->state == state;
    }
    
    return false;
}


/** Not equals comparison */
bool StandardState::operator!=(const CharacterState& x) const {
    
    return !operator==(x);
}


bool StandardState::operator<(const CharacterState &x) const {
    
    const StandardState* derivedX = static_cast<const StandardState*>(&x);
    if ( derivedX != NULL ) {
        unsigned int myState = state;
        unsigned int yourState = derivedX->state;
        
        while ( (myState & 1) == ( yourState & 1 )  ) {
            myState >>= 1;
            yourState >>= 1;
        }
        
        return (myState & 1) > ( yourState & 1 );
    }    
    
    return false;
}


void StandardState::operator++( void ) {
    state <<= 1;
}


void StandardState::operator--( void ) {
    state >>= 1;
}


void StandardState::addState(size_t pos) {
    state |= 1 << pos;
}



StandardState* StandardState::clone( void ) const {
    return new StandardState( *this );
}



/* Get class name of object */
const std::string& StandardState::getClassName(void) { 
    
    static std::string rbClassName = "Standard state";
    
	return rbClassName; 
}


size_t StandardState::getNumberOfStates( void ) const {
    return labels.size();
}


unsigned int StandardState::getNumberObservedStates(void) const  {
    
    unsigned int v = state;     // count the number of bits set in v
    unsigned int c;             // c accumulates the total bits set in v
    
    for (c = 0; v; v >>= 1)
    {
        c += v & 1;
    }
    
    return c;
}


const std::string& StandardState::getStateLabels( void ) const {
    
    return labels;
}

std::string StandardState::getStringValue(void) const  {
    
    std::string val = "";
    size_t size = labels.size();
    for (size_t i = 0; i < size; ++i) {
        unsigned long index = 0x1 << i;
        bool isSet = index & state;
        if (isSet) {
            val += labels[i];
        }
    }
    
    return val;
}



bool StandardState::isAmbiguous( void ) const {
    return getNumberObservedStates() > 1;
}


bool StandardState::isGapState( void ) const {
    return state == 0x0;
}


void StandardState::setGapState(bool tf) {
    if ( tf ) {
        state = 0;
    }
    else {
        state = 0;
        for (size_t i = 0; i < labels.size(); ++i) {
            state <<= 1;
            state |= 1;
        }
    }
}


void StandardState::setState(size_t pos, bool val) {
    
    state &= val << pos;
}


void StandardState::setState(char s) {
    
    size_t pos = labels.find(s);
    state = 1 << pos;
}

