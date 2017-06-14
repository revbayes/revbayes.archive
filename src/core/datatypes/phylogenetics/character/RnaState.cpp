#include "RnaState.h"
#include "RbException.h"
#include <assert.h>
#include <sstream>
#include <iostream>

using namespace RevBayesCore;

/** Default constructor */
<<<<<<< HEAD
RnaState::RnaState( size_t n ) : DiscreteCharacterState( 4 ) {
=======
RnaState::RnaState( size_t n ) : DiscreteCharacterState( 4 )
{
>>>>>>> development
    
}


/** Constructor that sets the observation */
<<<<<<< HEAD
RnaState::RnaState(const std::string &s) : DiscreteCharacterState( 4 ) {
=======
RnaState::RnaState(const std::string &s) : DiscreteCharacterState( 4 )
{
>>>>>>> development
    
    setState(s);
}


<<<<<<< HEAD
RnaState* RnaState::clone( void ) const {
=======
RnaState* RnaState::clone( void ) const
{
>>>>>>> development
    
    return new RnaState( *this );
}


<<<<<<< HEAD
void RnaState::setState(const std::string &symbol) {
    
    // we need to clear the bits first
    state.clear();
    
    char s = char( toupper( symbol[0] ) );
    
    switch ( s )
        {
        case '-':
            break;
        case 'A':
            state.set(0);
            index_single_state = 0;
            num_observed_states = 1;
            break;
        case 'C':
            state.set(1);
            index_single_state = 1;
            num_observed_states = 1;
            break;
        case 'M':
            state.set(0);
            state.set(1);
            num_observed_states = 2;
            break;
        case 'G':
            state.set(2);
            index_single_state = 2;
            num_observed_states = 1;
            break;
        case 'R':
            state.set(0);
            state.set(2);
            num_observed_states = 2;
            break;
        case 'S':
            state.set(1);
            state.set(2);
            num_observed_states = 2;
            break;
        case 'V':
            state.set(0);
            state.set(1);
            state.set(2);
            num_observed_states = 3;
            break;
        case 'U':
            state.set(3);
            index_single_state = 3;
            num_observed_states = 1;
            break;
        case 'W':
            state.set(0);
            state.set(3);
            num_observed_states = 2;
            break;
        case 'Y':
            state.set(1);
            state.set(3);
            num_observed_states = 2;
            break;
        case 'H':
            state.set(0);
            state.set(1);
            state.set(3);
            num_observed_states = 3;
            break;
        case 'K':
            state.set(2);
            state.set(3);
            num_observed_states = 2;
            break;
        case 'D':
            state.set(0);
            state.set(2);
            state.set(3);
            num_observed_states = 3;
            break;
        case 'B':
            state.set(1);
            state.set(2);
            state.set(3);
            num_observed_states = 3;
            break;
        case 'N':
            state.set(0);
            state.set(1);
            state.set(2);
            state.set(3);
            num_observed_states = 4;
            break;
            
        default:
            state.set(0);
            state.set(1);
            state.set(2);
            state.set(3);
            num_observed_states = 4;
        }
}

std::string RnaState::getDataType( void ) const {
    
    return "RNA";
}


std::string RnaState::getStateLabels( void ) const {
    
    static std::string labels = "ACGU";
    
    return labels;
}

std::string RnaState::getStringValue(void) const {
    
    if ( isMissingState() )
        {
        return "?";
        }
    
    if ( isGapState() )
        {
        return "-";
        }
    
    unsigned int val = 0x0;
    for ( int i=int(state.size())-1; i>=0; --i )
        {
        val <<= 1;
        if ( state.isSet(i) == true )
            {
            val |= 1;
            }
        }
    
    switch ( val )
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
=======
void RnaState::setState(const std::string &symbol)
{
    
    char s = char( toupper( symbol[0] ) );
    state = s;

}

std::string RnaState::getDataType( void ) const
{
    
    return "RNA";
}


std::string RnaState::getStateLabels( void ) const
{
    
    static std::string labels = "ACGU";
    
    return labels;
}

std::string RnaState::getStringValue(void) const
{
    
    return std::string(1,state);
}


void RnaState::addState(const std::string &symbol)
{
    //    ++num_observed_states;
    //
    //    std::string labels = getStateLabels();
    //    size_t pos = labels.find(symbol);
    //
    //    state.set( pos );
    //    index_single_state = pos;
    
    throw RbException("Cannot add a state to a RNA character!");
    
}


size_t RnaState::getNumberOfStates(void) const
{
    return 4;
}


RbBitSet RnaState::getState(void) const
{
    
    // we need to clear the bits first
    RbBitSet bs = RbBitSet(4);
    
    switch ( state )
    {
        case '-':
            break;
        case 'A':
            bs.set(0);
            break;
        case 'C':
            bs.set(1);
            break;
        case 'M':
            bs.set(0);
            bs.set(1);
            break;
        case 'G':
            bs.set(2);
            break;
        case 'R':
            bs.set(0);
            bs.set(2);
            break;
        case 'S':
            bs.set(1);
            bs.set(2);
            break;
        case 'V':
            bs.set(0);
            bs.set(1);
            bs.set(2);
            break;
        case 'U':
            bs.set(3);
            break;
        case 'W':
            bs.set(0);
            bs.set(3);
            break;
        case 'Y':
            bs.set(1);
            bs.set(3);
            break;
        case 'H':
            bs.set(0);
            bs.set(1);
            bs.set(3);
            break;
        case 'K':
            bs.set(2);
            bs.set(3);
            break;
        case 'D':
            bs.set(0);
            bs.set(2);
            bs.set(3);
            break;
        case 'B':
            bs.set(1);
            bs.set(2);
            bs.set(3);
            break;
        case 'N':
            bs.set(0);
            bs.set(1);
            bs.set(2);
            bs.set(3);
            break;
            
        default:
            bs.set(0);
            bs.set(1);
            bs.set(2);
            bs.set(3);
    }
    
    return bs;
}


bool RnaState::isGapState( void ) const
{
    return state == '-';
}


bool RnaState::isMissingState( void ) const
{
    return state == '?';
}


void RnaState::setGapState( bool tf )
{
    
    if ( tf == true )
    {
        state = '-';
    }
    
}


void RnaState::setMissingState( bool tf )
{
    
    if ( tf == true )
    {
        state = '?';
    }
    
}


void RnaState::setToFirstState(void)
{
    state = 'A';
}


void RnaState::setStateByIndex(size_t index)
{
    switch ( index )
    {
        case 0:
            state = 'A';
            break;
        case 1:
            state = 'C';
            break;
        case 2:
            state = 'G';
            break;
        case 3:
            state = 'U';
            break;
            
        default:
            state = '?';
            break;
    }
    
>>>>>>> development
}

