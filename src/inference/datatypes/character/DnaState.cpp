/**
 * @file
 * This file contains the implementation of DnaState, which is
 * the base class for the DNA character data type in RevBayes.
 *
 * @brief Implementation of DnaState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-05-24 09:58:04 +0200 (Thu, 24 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: DnaState.cpp 1568 2012-05-24 07:58:04Z hoehna $
 */

#include "DnaState.h"
#include "RbUtil.h"
#include <sstream>


/** Default constructor */
DnaState::DnaState(void) : DiscreteCharacterState(), state(0xFF) {
    
}


/** Copy constructor */
DnaState::DnaState(const DnaState& s) : DiscreteCharacterState(), state( s.state ) {
    
}


/** Constructor that sets the observation */
DnaState::DnaState(char s) : DiscreteCharacterState() {
    
    setState(s);
}


/** Equals comparison */
bool DnaState::operator==(const CharacterState& x) const {
    
    const DnaState* derivedX = dynamic_cast<const DnaState*>( &x );
    
    if (derivedX != NULL) {
        return derivedX->state == state;
    }
    
    return false;
}


/** Not equals comparison */
bool DnaState::operator!=(const CharacterState& x) const {
    
    return !operator==(x);
}


bool DnaState::operator<(const CharacterState &x) const {
    
    const DnaState* derivedX = static_cast<const DnaState*>(&x);
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


void DnaState::operator++( void ) {
    state <<= 1;
}


void DnaState::operator--( void ) {
    state >>= 1;
}


void DnaState::addState(size_t pos) {
    state |= 1 << pos;
}



DnaState* DnaState::clone( void ) const {
    return new DnaState( *this );
}



bool DnaState::isAmbiguous( void ) const {
    return getNumberObservedStates() > 1;
}


bool DnaState::isGapState( void ) const {
    return state == 0x0;
}



/* Get class name of object */
const std::string& DnaState::getClassName(void) { 
    
    static std::string rbClassName = "DNA state";
    
	return rbClassName; 
}


unsigned int DnaState::getNumberObservedStates(void) const  {
    
    unsigned int v = state;     // count the number of bits set in v
    unsigned int c;             // c accumulates the total bits set in v
    
    for (c = 0; v; v >>= 1)
    {
        c += v & 1;
    }
    
    return c;
}


size_t DnaState::getNumberOfStates( void ) const {
    return 4;
}


const std::string& DnaState::getStateLabels( void ) const {
    
    static std::string labels = "ACGT";
    
    return labels;
}

std::string DnaState::getStringValue(void) const  {
    
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
            return "T";
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


void DnaState::setGapState(bool tf) {
    if ( tf ) {
        state = 0x0;
    }
    else {
        state = 0xFF;
    }
}


void DnaState::setState(size_t pos, bool val) {
    
    state &= val << pos;
}

void DnaState::setState(char val) {
    
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
        case 'T':
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

