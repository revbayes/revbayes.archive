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
RnaState::RnaState(void) : DiscreteCharacterState(), state(0xf) {
    
}


/** Copy constructor */
RnaState::RnaState(const RnaState& s) : DiscreteCharacterState(), state( s.state ) {
    
}


/** Constructor that sets the observation */
RnaState::RnaState(char s) : DiscreteCharacterState() {
    
    //    setState(s);
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



RnaState* RnaState::clone( void ) const {
    return new RnaState( *this );
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


void RnaState::setState(size_t pos, bool val) {
    
    state &= val << pos;
}

