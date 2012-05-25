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
#include "RbUtil.h"
#include <sstream>


/** Default constructor */
AminoAcidState::AminoAcidState(void) : DiscreteCharacterState() {
    
//    setState('n');
}


/** Copy constructor */
AminoAcidState::AminoAcidState(const AminoAcidState& s) : DiscreteCharacterState() {
    
    state = s.state;
}


/** Constructor that sets the observation */
AminoAcidState::AminoAcidState(const char s) : DiscreteCharacterState() {
    
//    setState(s);
}


/** Equals comparison */
bool AminoAcidState::operator==(const CharacterState& x) const {
    
    const AminoAcidState* derivedX = static_cast<const AminoAcidState*>(&x);
    if ( derivedX != NULL ) {
        return state == derivedX->state;
    }    
    
    return false;
}


/** Not equals comparison */
bool AminoAcidState::operator!=(const CharacterState& x) const {
    
    return !operator==(x);
}


/** Set the observation */
void AminoAcidState::addState(size_t pos) {
    
    
}


/** Clone object */
AminoAcidState* AminoAcidState::clone(void) const {
    
	return  new AminoAcidState( *this );
}


/** Get class name of object */
const std::string& AminoAcidState::getClassName(void) { 
    
    static std::string rbClassName = "Amino Acid";
    
	return rbClassName; 
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



void AminoAcidState::setState(size_t pos, bool val) {
    
    state &= val << pos;
}