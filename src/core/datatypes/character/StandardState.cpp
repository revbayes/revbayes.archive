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
#include "RbException.h"
#include <sstream>
#include <cstdlib>
#include <cassert>

using namespace RevBayesCore;

/** Default constructor */
//StandardState::StandardState(void) : DiscreteCharacterState(), labels( "01" ), state(0x1) { }
StandardState::StandardState(void) : DiscreteCharacterState(), labels( "0123456789ABCDEFGHIJKLMNOPQRSTUV" ), state(0x1) { }


/** Default constructor */
StandardState::StandardState(const std::string &s) : DiscreteCharacterState(), labels( s ), state() {
    
}

StandardState::StandardState(const char& s, const std::string &l) : DiscreteCharacterState(), labels( l ), state() {
    setState(s);
}

StandardState::StandardState(const std::string& s, const std::string &l) : DiscreteCharacterState(), labels( l ), state() {
    setState(s);
}


/** Equals comparison */
bool StandardState::operator==(const CharacterState& x) const {
    
    const StandardState* derivedX = dynamic_cast<const StandardState*>( &x );
    
    if (derivedX != NULL) {
        return derivedX->labels == labels && derivedX->state == state;
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
        unsigned long myState = state;
        unsigned long yourState = derivedX->state;
        
        while ( (myState & 1) == ( yourState & 1 )  ) {
            myState >>= 1;
            yourState >>= 1;
        }
        
        return (myState & 1) > ( yourState & 1 );
    }    
    
    return false;
}


void StandardState::operator++( void )
{
    state <<= 1;
}


void StandardState::operator++( int i )
{
    state <<= 1;
}

void StandardState::operator+=( int i )
{
    state <<= i;
}

void StandardState::operator--( void )
{
    state >>= 1;
}


void StandardState::operator--( int i )
{
    state >>= 1;
}

void StandardState::operator-=( int i )
{
    state >>= i;
}

void StandardState::addState(char symbol) {
    state |= computeState( symbol );
}


std::string StandardState::getDatatype( void ) const {
    return "Standard";
}



StandardState* StandardState::clone( void ) const {
    return new StandardState( *this );
}


unsigned long StandardState::computeState(char symbol) const {
    
    size_t pos = labels.find(symbol);
    return pos;
}

size_t StandardState::getNumberOfStates( void ) const {
    return labels.size();
}


unsigned int StandardState::getNumberObservedStates(void) const  {
    
    unsigned long v = state;     // count the number of bits set in v
    unsigned int c;             // c accumulates the total bits set in v
    
    for (c = 0; v; v >>= 1)
    {
        c += v & 1;
    }
    
    return c;
}


unsigned long StandardState::getState( void ) const {
    return state;
}

size_t indexOfOnBit(size_t bitrep)
{
    
    // zero-valued bitreps should not exist
    assert(bitrep > 0);

    // the ambiguous state is the max value for size_t
    if ( (bitrep & (bitrep -1)) != 0)
    {
        size_t max = 0;
        return max - 1;
    }

    // return the unique flipped-bit index
    size_t index = 0;
    bitrep >>= 1;
    
    // there are still observed states left
    while ( bitrep != 0 )
    {
        bitrep >>= 1;
        ++index;
        
    }
    
    return index;
}
size_t setFirstNBitsOn(size_t n)
{
    assert(n <= 8*sizeof(size_t));
    size_t r = 0;
    size_t b = 1;
    for (size_t i = 0; i < n; ++i)
    {
        r |= b;
        b <<= 1;
    }
    return r;
}


size_t StandardState::getStateIndex( void ) const
{
    return indexOfOnBit(this->state);
}

const std::string& StandardState::getStateLabels( void ) const
{
    
    return labels;
}

std::string StandardState::getStringValue(void) const
{
    
    if ( isMissingState() )
    {
        return "?";
    }
    
    if ( isGapState() )
    {
        return "-";
    }
    
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


void StandardState::setState(size_t pos, bool val) {
    state &= ((unsigned long)val) << pos;
}


void StandardState::setState(char symbol) {
    
    size_t pos = labels.find(symbol);
    if (pos == std::string::npos) {
        throw RbException("Symbol \"" + std::string(1, symbol) + "\" not found in state labels \"" + this->labels + "\"");
    }
    state = (unsigned int)( 1 ) << pos;
}

void StandardState::setState(std::string s) {
    
    const char* tmp = s.c_str();
    
    for (size_t i = 0; i < s.size(); i++)
    {
        size_t pos = labels.find(tmp[i]);
        state = (unsigned int)( 1 ) << pos;
    }
}

void StandardState::setToFirstState( void )
{
    state = 0x01;
    
}

