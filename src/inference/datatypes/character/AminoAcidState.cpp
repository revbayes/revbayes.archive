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

const std::string AminoAcidState::stateLabels = "ARNDCQEGHILKMFPSTWYV";


/** Default constructor */
AminoAcidState::AminoAcidState(void) : CharacterStateDiscrete(20) {
    
    setState('n');
}


/** Copy constructor */
AminoAcidState::AminoAcidState(const AminoAcidState& s) : CharacterStateDiscrete(20) {
    
    value = s.value;
}


/** Constructor that sets the observation */
AminoAcidState::AminoAcidState(const char s) : CharacterStateDiscrete(20) {
    
    setState(s);
}


/** Equals comparison */
bool AminoAcidState::operator==(const Character& x) const {
    
    const AminoAcidState* derivedX = static_cast<const AminoAcidState*>(&x);
    for (size_t i=0; i<numStates; i++) 
    {
        if ( value[i] != derivedX->value[i] )
            return false;
    }
    return true;
}


/** Not equals comparison */
bool AminoAcidState::operator!=(const Character& x) const {
    
    return !operator==(x);
}


/** Set the observation */
void AminoAcidState::addState(const char s) {
    
    // look for matches against the state label static vector
    char c = toupper(s);
    size_t numMatches = 0;
    for (size_t i=0; i<numStates; i++)
    {
        if ( c == stateLabels[i] )
        {
            value[i] = true;
            numMatches++;
        }
    }
    
    // if there are no matches, we assume that the character state was something like
    // a '?', 'n', or '-' and set all of the flags to true, to indicate complete ambiguity
    if (numMatches == 0)
    {
        for (size_t i=0; i<numStates; i++)
            value[i] = true;
    }
    if (s == '-')
        isGapState = true;
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

/** Get class type spec describing type of object */
const TypeSpec& AminoAcidState::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( CharacterStateDiscrete::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& AminoAcidState::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get value */
const char AminoAcidState::getState(void) const {
    
    char c = '?';
    size_t numMatches = 0;
    for (size_t i=0; i<numStates; i++)
    {
        if ( value[i] == true )
        {
            c = stateLabels[i];
            numMatches++;
        }
    }
    if (numMatches > 1)
        c = '?';
    return c;
}

std::string AminoAcidState::getStringValue(void) const  {
    
    char c = getState();
    std::string s = "";
    s += c;
    return s;
}


/** Print information for the user */
void AminoAcidState::printValue(std::ostream &o) const {
    
    o << getState();
}


void AminoAcidState::setState(const char s) {
    
    // wipe the value clean, setting all bool flags to false
    for (size_t i=0; i<numStates; i++)
        value[i] = false;
    addState(s);
}