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
#include "RbException.h"
#include <assert.h>
#include <sstream>
#include <iostream>

using namespace RevBayesCore;

/** Default constructor */
DnaState::DnaState(void) : DiscreteCharacterState(), state( char(0xFF) ), stateIndex(0xFF) {
    
}


/** Copy constructor */
DnaState::DnaState(const DnaState& s) : DiscreteCharacterState(), state( s.state ), stateIndex( s.stateIndex ) {
    
}


/** Constructor that sets the observation */
DnaState::DnaState(char s) : DiscreteCharacterState() {
    
    assert( s <= 15 );
    
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
    if ( derivedX != NULL ) 
    {
        char myState = state;
        char yourState = derivedX->state;
        
        while ( myState != yourState && (myState & 1) == ( yourState & 1 )  ) 
        {
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


void DnaState::operator++( int i ) {

    state <<= 1;

}


void DnaState::operator--( void ) {
    
    state >>= 1;

}


void DnaState::operator--( int i ) {

    state >>= 1;

}


void DnaState::addState(char symbol) {

    state |= computeState( symbol );

}



DnaState* DnaState::clone( void ) const {
    
    return new DnaState( *this );
}


unsigned int DnaState::computeState(char symbol) const {
    
    if ( symbol == '?')
    {
        throw RbException( "Unknown char" );
    }
    
    symbol = char( toupper( symbol ) );
    switch ( symbol )
    {
        case '-':
            return 0x00;
        case 'A':
            return 0x01;
        case 'C':
            return 0x02;
        case 'M':
            return 0x03;
        case 'G':
            return 0x04;
        case 'R':
            return 0x05;
        case 'S':
            return 0x06;
        case 'V':
            return 0x07;
        case 'T':
            return 0x08;
        case 'W':
            return 0x09;
        case 'Y':
            return 0x0A;
        case 'H':
            return 0x0B;
        case 'K':
            return 0x0C;
        case 'D':
            return 0x0D;
        case 'B':
            return 0x0E;
        case 'N':
            return 0x0F;
            
        default:
            return 0x0F;
    }
}


std::string DnaState::getDatatype( void ) const {
    
    return "DNA";
}


unsigned int DnaState::getNumberObservedStates(void) const  {
    
    char v = state;     // count the number of bits set in v
    char c;             // c accumulates the total bits set in v
    
    for (c = 0; v; v >>= 1)
    {
        c += v & 1;
    }
    
    return (unsigned int)c;
}


size_t DnaState::getNumberOfStates( void ) const {
    
    return 4;
}


unsigned long DnaState::getState( void ) const {
    
    return (unsigned long)state;
}

size_t DnaState::getStateIndex( void ) const {
    
    return stateIndex;
}

const std::string& DnaState::getStateLabels( void ) const {
    
    static std::string labels = "ACGT";
    
    return labels;
}

std::string DnaState::getStringValue(void) const  {
    
    switch ( state ) 
    {
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



bool DnaState::isAmbiguous( void ) const {
    
    return getNumberObservedStates() > 1;
}


bool DnaState::isGapState( void ) const {
    
    return state == 0x0;
}


void DnaState::setGapState(bool tf) {
        
    if ( tf ) 
    {
        state = 0x0;
    }
    else 
    {
        state = 0xF;
    }
}


void DnaState::setState(size_t pos, bool val) {
    
    state &= val << pos;
    stateIndex = (unsigned)pos;
    
}

void DnaState::setState(char symbol) 
{
    state = char( computeState( symbol ) );
    switch ( state )
    {
        case 0x1: { stateIndex = 0; break; }
        case 0x2: { stateIndex = 1; break; }
        case 0x4: { stateIndex = 2; break; }
        case 0x8: { stateIndex = 3; break; }
        default: stateIndex = 4;
    }
    
}

void DnaState::setToFirstState( void ) {
    
    state = 0x1;

}

