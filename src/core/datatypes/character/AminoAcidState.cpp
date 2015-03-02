/**
 * @file
 * This file contains the implementation of AminoAcidState, which is
 * the class for the Amino Acid character data type in RevBayes.
 *
 * @brief Implementation of AminoAcidState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-05-24 09:58:04 +0200 (Thu, 24 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: AminoAcidState.cpp 1568 2012-05-24 07:58:04Z hoehna $
 */

#include "AminoAcidState.h"
#include <sstream>

using namespace RevBayesCore;

/** Default constructor */
AminoAcidState::AminoAcidState(void) : DiscreteCharacterState() {
    
    setState('-');
}


/** Copy constructor */
AminoAcidState::AminoAcidState(const AminoAcidState& s) : DiscreteCharacterState() {
    
    state = s.state;
    stateIndex = s.stateIndex;
}


/** Constructor that sets the observation */
AminoAcidState::AminoAcidState(char s) : DiscreteCharacterState() {
    
    setState(s);
}


/* Equals comparison */
bool AminoAcidState::operator==(const CharacterState& x) const {
    
    const AminoAcidState* derivedX = static_cast<const AminoAcidState*>(&x);
    if ( derivedX != NULL ) {
        return state == derivedX->state;
    }    
    
    return false;
}


/* Not equals comparison */
bool AminoAcidState::operator!=(const CharacterState& x) const {
    
    return !operator==(x);
}


bool AminoAcidState::operator<(const CharacterState &x) const {
    
    const AminoAcidState* derivedX = static_cast<const AminoAcidState*>(&x);
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

void AminoAcidState::operator++( void )
{
    state <<= 1;
    ++stateIndex;
}

void AminoAcidState::operator++( int i )
{
    state <<= 1;
    ++stateIndex;
}

void AminoAcidState::operator+=( int i )
{
    state <<= i;
    stateIndex += i;
}

void AminoAcidState::operator--( void )
{
    state >>= 1;
    --stateIndex;
}


void AminoAcidState::operator--( int i )
{
    state >>= 1;
    --stateIndex;
}

void AminoAcidState::operator-=( int i )
{
    state >>= i;
    stateIndex -= i;
}

void AminoAcidState::addState(char symbol)
{
    state |= computeState( symbol );
}


/* Clone object */
AminoAcidState* AminoAcidState::clone(void) const {
    
	return  new AminoAcidState( *this );
}


unsigned int AminoAcidState::computeState(char symbol) const {
    /* A R N D C Q E G H I L K M F P S T W Y V */
    symbol = char( toupper( symbol ) );
    
    
    switch ( symbol ) {
        case '-':
            return 0x00000;
        case 'A':
            return 0x00001;
        case 'R':
            return 0x00002;
        case 'N':
            return 0x00004;
        case 'D':
            return 0x00008;
        case 'C':
            return 0x00010;
        case 'Q':
            return 0x00020;
        case 'E':
            return 0x00040;
        case 'G':
            return 0x00080;
        case 'H':
            return 0x00100;
        case 'I':
            return 0x00200;
        case 'L':
            return 0x00400;
        case 'K':
            return 0x00800;
        case 'M':
            return 0x01000;
        case 'F':
            return 0x02000;
        case 'P':
            return 0x04000;
        case 'S':
            return 0x08000;
        case 'T':
            return 0x10000;
        case 'W':
            return 0x20000;
        case 'Y':
            return 0x40000;
        case 'V':
            return 0x80000;
            
        default:
            return 0x8FFFF;
    }
}


std::string AminoAcidState::getDatatype( void ) const {
    return "Protein";
}


unsigned int AminoAcidState::getNumberObservedStates( void ) const {
    return 1;
}


size_t AminoAcidState::getNumberOfStates( void ) const {
    return 20;
}


unsigned long AminoAcidState::getState( void ) const {
    return state;
}

size_t AminoAcidState::getStateIndex( void ) const
{
    return stateIndex;
}

const std::string& AminoAcidState::getStateLabels( void ) const {
    
    static const std::string stateLabels = "ARNDCQEGHILKMFPSTWYV";
   
    return stateLabels;
}

std::string AminoAcidState::getStringValue(void) const  {
    
    switch ( state ) {
        case 0x00000:
            return "-";
        case 0x00001:
            return "A";
        case 0x00002:
            return "R";
        case 0x00004:
            return "N";
        case 0x00008:
            return "D";
        case 0x00010:
            return "C";
        case 0x00020:
            return "Q";
        case 0x00040:
            return "E";
        case 0x00080:
            return "G";
        case 0x00100:
            return "H";
        case 0x00200:
            return "I";
        case 0x00400:
            return "L";
        case 0x00800:
            return "K";
        case 0x01000:
            return "M";
        case 0x02000:
            return "F";
        case 0x04000:
            return "P";
        case 0x08000:
            return "S";
        case 0x10000:
            return "T";
        case 0x20000:
            return "W";
        case 0x40000:
            return "Y";
        case 0x80000:
            return "V";
            
        default:
            return "?";
    }
}


bool AminoAcidState::isAmbiguous( void ) const {
    return getNumberObservedStates() > 1;
}


bool AminoAcidState::isGapState( void ) const {
    return state == 0x0;
}


void AminoAcidState::setGapState(bool tf) {
    if ( tf ) {
        state  = computeState('-');
    }
    else {
        state = computeState('?');
    }
}

void AminoAcidState::setState(size_t pos, bool val) {
    
    unsigned x = (unsigned)val << pos;
    
//    state &= ((unsigned int)val) << pos;
    state ^= x;
    stateIndex = pos;
}


void AminoAcidState::setState(char symbol) {
    state = computeState( symbol );
    
    switch ( state )
    {
        case 0x00001: { stateIndex = 0; break; }
        case 0x00002: { stateIndex = 1; break; }
        case 0x00004: { stateIndex = 2; break; }
        case 0x00008: { stateIndex = 3; break; }
        case 0x00010: { stateIndex = 4; break; }
        case 0x00020: { stateIndex = 5; break; }
        case 0x00040: { stateIndex = 6; break; }
        case 0x00080: { stateIndex = 7; break; }
        case 0x00100: { stateIndex = 8; break; }
        case 0x00200: { stateIndex = 9; break; }
        case 0x00400: { stateIndex = 10; break; }
        case 0x00800: { stateIndex = 11; break; }
        case 0x01000: { stateIndex = 12; break; }
        case 0x02000: { stateIndex = 13; break; }
        case 0x04000: { stateIndex = 14; break; }
        case 0x08000: { stateIndex = 15; break; }
        case 0x10000: { stateIndex = 16; break; }
        case 0x20000: { stateIndex = 17; break; }
        case 0x40000: { stateIndex = 18; break; }
        case 0x80000: { stateIndex = 19; break; }
        default: stateIndex = -1;
    }
}


void AminoAcidState::setToFirstState( void ) {
    state = 0x1;
    stateIndex = 0;
}


