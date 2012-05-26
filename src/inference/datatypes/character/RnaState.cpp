/**
 * @file
 * This file contains the implementation of RnaState, which is
 * the base class for the Rna character data type in RevBayes.
 *
 * @brief Implementation of RnaState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-05-24 09:58:04 +0200 (Thu, 24 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: RnaState.cpp 1568 2012-05-24 07:58:04Z hoehna $
 */

#include "RnaState.h"
#include "RbUtil.h"
#include <sstream>


/** Default constructor */
RnaState::RnaState(void) : DiscreteCharacterState(), state(0xFF) {
    
}


/** Copy constructor */
RnaState::RnaState(const RnaState& s) : DiscreteCharacterState(), state( s.state ) {
    
}


/** Constructor that sets the observation */
RnaState::RnaState(char s) : DiscreteCharacterState() {
    
    setState(s);
}


/** Equals comparison */
bool RnaState::operator==(const CharacterState& x) const {
    
    const RnaState* derivedX = dynamic_cast<const RnaState*>( &x );
    
    if (derivedX != NULL) {
        return derivedX->state == state;
    }
    
    return false;
}


/** Not equals comparison */
bool RnaState::operator!=(const CharacterState& x) const {
    
    return !operator==(x);
}


bool RnaState::operator<(const CharacterState &x) const {
    
    const RnaState* derivedX = static_cast<const RnaState*>(&x);
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


void RnaState::operator++( void ) {
    state <<= 1;
}


void RnaState::operator--( void ) {
    state >>= 1;
}


void RnaState::addState(size_t pos) {
    state |= 1 << pos;
}



RnaState* RnaState::clone( void ) const {
    return new RnaState( *this );
}



/**Get class name of object */
const std::string& RnaState::getClassName(void) { 
    
    static std::string rbClassName = "RNA state";
    
	return rbClassName; 
}


unsigned int RnaState::getNumberObservedStates(void) const  {
    
    unsigned int v = state;     // count the number of bits set in v
    unsigned int c;             // c accumulates the total bits set in v
    
    for (c = 0; v; v >>= 1)
    {
        c += v & 1;
    }
    
    return c;
}


size_t RnaState::getNumberOfStates( void ) const {
    return 4;
}


unsigned int RnaState::getState( void ) const {
    return state;
}


const std::string& RnaState::getStateLabels( void ) const {
    
    static std::string labels = "ACGU";
    
    return labels;
}

std::string RnaState::getStringValue(void) const  {
    
    switch ( state ) {
        case 0x0:
            return "-";
        case 0x1:
            return "A";
        case 0x2:
            return "C";
        case 0x3:
            return "M";
        case 0x4:
            return "G";
        case 0x5:
            return "R";
        case 0x6:
            return "S";
        case 0x7:
            return "V";
        case 0x8:
            return "U";
        case 0x9:
            return "W";
        case 0xA:
            return "Y";
        case 0xB:
            return "H";
        case 0xC:
            return "K";
        case 0xD:
            return "D";
        case 0xE:
            return "B";
        case 0xF:
            return "N";
            
        default:
            return "?";
    }
}



bool RnaState::isAmbiguous( void ) const {
    return getNumberObservedStates() > 1;
}


bool RnaState::isGapState( void ) const {
    return state == 0x0;
}


void RnaState::setGapState(bool tf) {
    if ( tf ) {
        state = 0x0;
    }
    else {
        state = 0xFF;
    }
}


void RnaState::setState(size_t pos, bool val) {
    
    state &= val << pos;
}

void RnaState::setState(char val) {
    
    val = toupper( val );
    switch ( val ) {
        case '-':
            state = 0x00;
            break;
        case 'A':
            state = 0x01;
            break;
        case 'C':
            state = 0x02;
            break;
        case 'M':
            state = 0x03;
            break;
        case 'G':
            state = 0x04;
            break;
        case 'R':
            state = 0x05;
            break;
        case 'S':
            state = 0x06;
            break;
        case 'V':
            state = 0x07;
            break;
        case 'U':
            state = 0x08;
            break;
        case 'W':
            state = 0x09;
            break;
        case 'Y':
            state = 0x0A;
            break;
        case 'H':
            state = 0x0B;
            break;
        case 'K':
            state = 0x0C;
            break;
        case 'D':
            state = 0x0D;
            break;
        case 'B':
            state = 0x0E;
            break;
        case 'N':
            state = 0x0F;
            break;
            
        default:
            state = 0xFF;
    }
}

