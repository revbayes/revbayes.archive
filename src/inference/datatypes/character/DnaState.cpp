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
DnaState::DnaState(void) : DiscreteCharacterState(), state(0xf) {
    
}


/** Copy constructor */
DnaState::DnaState(const DnaState& s) : DiscreteCharacterState(), state( s.state ) {
    
}


/** Constructor that sets the observation */
DnaState::DnaState(char s) : DiscreteCharacterState() {
    
//    setState(s);
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



DnaState* DnaState::clone( void ) const {
    return new DnaState( *this );
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
            return "N";
    }
}


void DnaState::setState(size_t pos, bool val) {
    
    state &= val << pos;
}

